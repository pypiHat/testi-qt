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

  void run ();

private:
  GstElement * pipeline_;
};

SetPlaying::SetPlaying (GstElement * pipeline)
{
  this->pipeline_ = pipeline ? static_cast<GstElement *> (gst_object_ref (pipeline)) : NULL;
}

SetPlaying::~SetPlaying ()
{
  if (this->pipeline_)
    gst_object_unref (this->pipeline_);
}

void
SetPlaying::run ()
{
  if (this->pipeline_)
  {
      qDebug() << "omaddd";
    gst_element_set_state (this->pipeline_, GST_STATE_PLAYING);
  }
}

int main(int argc, char *argv[])
{
    setenv("GST_DEBUG_DUMP_DOT_DIR" , "/tmp" , 1);

  int ret;
  gst_init (&argc, &argv);

  {
    QGuiApplication app(argc, argv);

    GstElement *pipeline = gst_pipeline_new (NULL);
//#define MULTIFILESRC
//#define IMAGEFREEZE
//#define VIDEOTESTSRC
//#define IMAGEFREEZE1
#define TESTAUTOVIDEOSINK

#ifdef MULTIFILESRC
    //    gst-launch-1.0 filesrc location=/home/mohammad/Videos/sintel_trailer-480p.mp4 ! qtdemux ! avdec_h264 ! autovideosink

    GstElement *src = gst_element_factory_make ("multifilesrc", NULL);

    g_object_set(src , "location" , "/home/mohammad/Pictures/myimage%d.jpeg" ,NULL);
    g_object_set(src , "loop" , true, "index", 0 , NULL);

//    g_object_set(src , "caps" , "image/jpeg,framerate=\(fraction\)12/1" , NULL);
    GstElement *jpegdec = gst_element_factory_make("jpegdec" , NULL);

    GstElement *glupload = gst_element_factory_make ("glupload", NULL);
    GstElement *glcolorconvert = gst_element_factory_make("glcolorconvert",NULL);
    GstElement *sink = gst_element_factory_make ("qmlglsink", NULL);

    g_assert (src && jpegdec && glupload && glcolorconvert && sink);

    gst_bin_add_many (GST_BIN (pipeline), src, jpegdec, glupload, glcolorconvert, sink, NULL);
    if(gst_element_link_many (src, jpegdec, glupload, glcolorconvert, sink, NULL)
       != TRUE) {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipeline);
        return 1;
    }

#endif
#ifdef IMAGEFREEZE
    GstElement *src = gst_element_factory_make ("filesrc", NULL);

    g_object_set(src , "location" , "/home/mohammad/Pictures/myimage0.jpeg" ,NULL);
    GstElement *jpegdec = gst_element_factory_make("jpegdec" , NULL);
    GstElement *imagefreeze = gst_element_factory_make("imagefreeze" , NULL);
    GstElement *glupload = gst_element_factory_make ("glupload", NULL);
    GstElement *sink = gst_element_factory_make ("qmlglsink", NULL);
    g_assert(jpegdec);

    g_assert(imagefreeze);
    g_assert (src && glupload && sink);

    gst_bin_add_many (GST_BIN (pipeline), src, jpegdec, imagefreeze, glupload, sink, NULL);

    if (!src || !jpegdec ||
        !glupload || !sink)
    {
        g_printerr("Not all elements could be created.\n");
        return 1;
    }
    if (gst_element_link_many(src , jpegdec  , imagefreeze, glupload, sink, NULL)
          != TRUE)
    {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipeline);
    }
#endif
#ifdef IMAGEFREEZE1
    GstElement *src = gst_element_factory_make ("filesrc", NULL);
    g_object_set(src , "location" , "/home/mohammad/Pictures/myimage0.jpeg" ,NULL);
    GstElement *jpegdec = gst_element_factory_make("jpegdec" , NULL);
    GstElement *imagefreeze = gst_element_factory_make("imagefreeze" , NULL);

    GstElement *glupload = gst_element_factory_make ("glupload", NULL);
    /* the plugin must be loaded before loading the qml file to register the
     * GstGLVideoItem qml item */
    GstElement *sink = gst_element_factory_make ("qmlglsink", NULL);

    g_assert (src && jpegdec && imagefreeze && glupload && sink);

    gst_bin_add_many (GST_BIN (pipeline), src, jpegdec, imagefreeze, glupload, sink, NULL);
    if(gst_element_link_many (src, jpegdec, imagefreeze, glupload, sink, NULL)
       != TRUE) {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipeline);
        return 1;
    }

#endif

#ifdef TESTAUTOVIDEOSINK
    GstElement *src = gst_element_factory_make ("filesrc", NULL);
    g_object_set(src , "location" , "/home/mohammad/Pictures/myimage0.jpeg" ,NULL);
    GstElement *jpegdec = gst_element_factory_make("jpegdec" , NULL);
    GstElement *imagefreeze = gst_element_factory_make("imagefreeze" , NULL);

    GstElement *videorate = gst_element_factory_make("videorate" , NULL);

    GstElement *videorateCapsElement = gst_element_factory_make("capsfilter", NULL);
    GstCaps *videorateCaps = gst_caps_from_string("video/x-raw,framerate=1/10");
    g_object_set(videorateCapsElement, "caps", videorateCaps, NULL);

    GstElement *glupload = gst_element_factory_make ("glupload", NULL);
    GstElement *glcolorconvert = gst_element_factory_make("glcolorconvert",NULL);

    /* the plugin must be loaded before loading the qml file to register the
     * GstGLVideoItem qml item */
    GstElement *sinkqml = gst_element_factory_make ("qmlglsink", NULL);
    GstElement *autovideosink = gst_element_factory_make ("autovideosink", NULL);

    GstElement *sink = sinkqml;
    g_assert (src && jpegdec && imagefreeze
              && videorate && videorateCapsElement
              && glupload
              && glcolorconvert
              && sink);

    gst_bin_add_many (GST_BIN (pipeline), src, jpegdec, imagefreeze, videorate,
                      videorateCapsElement, glupload, glcolorconvert, sink, NULL);
    if(gst_element_link_many (src, jpegdec, imagefreeze, videorate, videorateCapsElement,
                              glupload, glcolorconvert, sink, NULL)
       != TRUE) {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipeline);
        return 1;
    }

#endif

#ifdef VIDEOTESTSRC
    GstElement *src = gst_element_factory_make ("videotestsrc", NULL);
    GstElement *glupload = gst_element_factory_make ("glupload", NULL);
    /* the plugin must be loaded before loading the qml file to register the
     * GstGLVideoItem qml item */
    GstElement *sink = gst_element_factory_make ("qmlglsink", NULL);

    g_assert (src && glupload && sink);

    gst_bin_add_many (GST_BIN (pipeline), src, glupload, sink, NULL);
    gst_element_link_many (src, glupload, sink, NULL);

#endif
    GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "piplineLinked");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QQuickItem *videoItem;
    QQuickWindow *rootObject;

    /* find and set the videoItem on the sink */
    rootObject = static_cast<QQuickWindow *> (engine.rootObjects().first());
    videoItem = rootObject->findChild<QQuickItem *> ("videoItem");
    g_assert (videoItem);
    g_object_set(sink, "widget", videoItem, NULL);

    rootObject->scheduleRenderJob (new SetPlaying (pipeline),
        QQuickWindow::BeforeSynchronizingStage);

    ret = app.exec();
    qDebug() << "after";
    GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "piplinePlay");

    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (pipeline);

  }

  gst_deinit ();

  return ret;
}
