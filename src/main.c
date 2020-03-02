#include <gst/gst.h>
#include <gst/gl/gl.h>

static void
_on_destroy (gpointer data)
{
  g_print ("Context %s is being destroyed\n", (gchar *)data);
  g_free (data);
}

int
main (int argc, char** argv)
{
  gboolean use_analysis = FALSE;
  GstGLContext *context;
  gchar *name, *el_name;
  GstElement *bin, *src, *conv, *scrut, *sink;
  
  gst_init (&argc, &argv);

  if (argc < 2 || g_strcmp0(argv[1], "-a") == 0)
    use_analysis = TRUE;

  bin = gst_pipeline_new ("pipeline");
  src = gst_element_factory_make ("gltestsrc", NULL);
  conv = gst_element_factory_make ("glcolorconvert", NULL);
  if (use_analysis)
    scrut = gst_element_factory_make ("videoanalysis", NULL);
  else
    scrut = gst_element_factory_make ("identity", NULL);
  sink = gst_element_factory_make ("fakesink", NULL);

  gst_bin_add_many (GST_BIN(bin), src, conv, scrut, sink, NULL);

  if (!gst_element_link_many (src, conv, scrut, sink, NULL))
    {
      g_print ("failed to link elements");
      exit(-1);
    }

  el_name = gst_object_get_name(GST_OBJECT(conv));
  g_object_set_data_full (G_OBJECT(conv),
                          "on_destroy",
                          el_name,
                          _on_destroy);

  gst_element_set_state (bin, GST_STATE_PLAYING);

  sleep (5);
  
  g_print ("clock_attach_wait returned\n");
  g_object_get (conv, "context", &context, NULL);
  name = gst_object_get_name(GST_OBJECT(context));
  g_print ("Found context %s\n", name);
  g_object_set_data_full (G_OBJECT(context),
                          "on_destroy",
                          name,
                          _on_destroy);
  gst_object_unref (context);

  g_print ("Context refcount %d\n", GST_OBJECT_REFCOUNT_VALUE(context));

  sleep (5);
  g_print ("clock_id_wait returned\n");
  gst_element_set_state (bin, GST_STATE_NULL);

  gst_object_unref (G_OBJECT (bin));
  g_print ("Context refcount %d\n", GST_OBJECT_REFCOUNT_VALUE(context));
  
  sleep (5);
  
}
