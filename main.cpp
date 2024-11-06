#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQuickItem>
#include <QRunnable>
#include <gst/gst.h>

class SetPlaying : public QRunnable
{
public:
  SetPlaying(GstElement *);
  ~SetPlaying();

  void run();

private:
  GstElement *pipeline_;
};

SetPlaying::SetPlaying(GstElement *pipeline)
{
  this->pipeline_ = pipeline ? static_cast<GstElement *>(gst_object_ref(pipeline)) : NULL;
}

SetPlaying::~SetPlaying()
{
  if (this->pipeline_)
    gst_object_unref(this->pipeline_);
}

void SetPlaying::run()
{
  if (this->pipeline_)
  {
    qDebug() << "Setting pipeline to PLAYING state";
    gst_element_set_state(this->pipeline_, GST_STATE_PLAYING);
  }
}

int main(int argc, char *argv[])
{
  // Set environment variable for GStreamer debug output
  setenv("GST_DEBUG_DUMP_DOT_DIR", "/tmp", 1);

  int ret;
  gst_init(&argc, &argv);

  {
    QGuiApplication app(argc, argv);

    GstElement *pipeline = gst_pipeline_new("video-pipeline");

    // Use a `filesrc` to load a video file
    GstElement *src = gst_element_factory_make("filesrc", "file-source");
    g_object_set(src, "location", "/path/to/your/video.mp4", NULL);  // Specify your video file path here

    // Set up the demuxer and decoder elements for the video file
    GstElement *demuxer = gst_element_factory_make("qtdemux", "demuxer"); // e.g., qtdemux for MP4
    GstElement *decoder = gst_element_factory_make("avdec_h264", "decoder"); // H.264 decoder

    // Additional pipeline elements
    GstElement *videoconvert = gst_element_factory_make("videoconvert", "converter");
    GstElement *glupload = gst_element_factory_make("glupload", "gl-upload");
    GstElement *sink = gst_element_factory_make("qmlglsink", "qml-sink");

    // Verify that all elements were created
    if (!pipeline || !src || !demuxer || !decoder || !videoconvert || !glupload || !sink) {
      g_printerr("Failed to create one or more GStreamer elements.\n");
      return -1;
    }

    // Assemble the pipeline
    gst_bin_add_many(GST_BIN(pipeline), src, demuxer, decoder, videoconvert, glupload, sink, NULL);
    
    // Link the elements
    if (!gst_element_link(src, demuxer)) {
      g_printerr("Failed to link src and demuxer.\n");
      gst_object_unref(pipeline);
      return -1;
    }

    // Connect demuxer to decoder dynamically
    g_signal_connect(demuxer, "pad-added", G_CALLBACK(+[](GstElement *src, GstPad *new_pad, GstElement *decoder) {
      GstPad *sink_pad = gst_element_get_static_pad(decoder, "sink");
      gst_pad_link(new_pad, sink_pad);
      gst_object_unref(sink_pad);
    }), decoder);

    // Link the rest of the pipeline
    if (!gst_element_link_many(decoder, videoconvert, glupload, sink, NULL)) {
      g_printerr("Failed to link decoder to sink.\n");
      gst_object_unref(pipeline);
      return -1;
    }

    // Start Qt application and load QML
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QQuickWindow *rootObject = static_cast<QQuickWindow *>(engine.rootObjects().first());
    QQuickItem *videoItem = rootObject->findChild<QQuickItem *>("videoItem");
    if (!videoItem) {
      g_printerr("Failed to find videoItem in QML.\n");
      return -1;
    }
    g_object_set(sink, "widget", videoItem, NULL);

    // Schedule the pipeline to play
    rootObject->scheduleRenderJob(new SetPlaying(pipeline), QQuickWindow::BeforeSynchronizingStage);

    ret = app.exec();

    // Clean up GStreamer pipeline after Qt application ends
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
  }

  gst_deinit();

  return ret;
}
