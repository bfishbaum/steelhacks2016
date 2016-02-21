#include <pebble.h>
#include "windows/dialog_choice_window.h"

static Window *s_main_window;
static TextLayer *s_output_layer;
static BitmapLayer *s_icon_layer;

static GBitmap *s_icon_bitmap;
int prevX = 0;
int prevY = 0;
int prevZ = 0;
int vibed = 0;
int stopVibed = 0;

static void data_handler(AccelData *data, uint32_t num_samples) {
  // Long lived buffer
  static char s_buffer[128];
  int avgX = (data[0].x+data[1].x+data[2].x)/3;
  int avgY = (data[0].y+data[1].y+data[2].y)/3;
  int avgZ = (data[0].z+data[1].z+data[2].z)/3;

  if(((abs(prevX-avgX)>2100)||(abs(prevY-avgY)>2100)||(abs(prevZ-avgZ)>2100))&&(vibed==0&&stopVibed!=2)){
    if(vibed!=500){
      vibed = 3;
      stopVibed++;
    };
    // Vibe pattern: ON for 200ms, OFF for 100ms, ON for 400ms:
    static const uint32_t const segments[] = { 200, 50, 200 };
    VibePattern pat = {
      .durations = segments,
      .num_segments = ARRAY_LENGTH(segments),
    };
    vibes_enqueue_custom_pattern(pat);
    dialog_choice_window_push();
  }
  if(vibed>0){vibed--;}
  prevX = abs(avgX);
  prevY = abs(avgY);
  prevZ = abs(avgZ);
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  s_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_OWL);
  const GEdgeInsets icon_insets = {.top = 0, .right = 0, .bottom = 45, .left = 0};
  s_icon_layer = bitmap_layer_create(grect_inset(window_bounds, icon_insets));
  bitmap_layer_set_bitmap(s_icon_layer, s_icon_bitmap);
  bitmap_layer_set_compositing_mode(s_icon_layer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_icon_layer));

  const GEdgeInsets label_insets = {.top = 112, .right = 0, .left = 0};
  s_output_layer = text_layer_create(grect_inset(window_bounds, label_insets));
  text_layer_set_text(s_output_layer, "Night Owl is running...");
  text_layer_set_background_color(s_output_layer, GColorClear);
  text_layer_set_text_alignment(s_output_layer, GTextAlignmentCenter);
  text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_overflow_mode(s_output_layer, GTextOverflowModeWordWrap);
  layer_add_child(window_layer, text_layer_get_layer(s_output_layer));
}

static void main_window_unload(Window *window) {
  // Destroy output TextLayer
  text_layer_destroy(s_output_layer);
}

static void init() {
  // Create main Window
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);
  app_message_open(128,128);
  // Subscribe to the accelerometer data service
  int num_samples = 3;
  accel_data_service_subscribe(num_samples, data_handler);
  // Choose update rate
  accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);

}

static void deinit() {
  // Destroy main Window
  window_destroy(s_main_window);
  accel_data_service_unsubscribe();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
