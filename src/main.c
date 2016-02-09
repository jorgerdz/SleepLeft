#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_time_diff_layer;
static TextLayer *s_date_layer;
static TextLayer *s_temp_layer;

static char date_str[] = "Xxxxxxxxxxxxxxxxxxxxxxx 00";

static void get_time_diff(struct tm *now, int hourToCompare, int minToCompare, struct tm *diff) {
	if(minToCompare<now->tm_min){
		hourToCompare--;
		minToCompare+=60;
	}
	if(hourToCompare<now->tm_hour){
		hourToCompare+=24;
  }
	
  diff->tm_min=minToCompare-now->tm_min;
  diff->tm_hour=hourToCompare-now->tm_hour;
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%R" : "%l:%M", tick_time);
	strftime(date_str, sizeof(date_str), "%b %e", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
	text_layer_set_text(s_date_layer, date_str);
	text_layer_set_text(s_temp_layer, "24°C");
	
	struct tm *diff = localtime(&temp);
	struct tm *unformattedTime = localtime(&temp);
	get_time_diff(unformattedTime, 9, 0, diff);
	  // Write the current hours and minutes into a buffer
  static char s_buffer2[8];
  strftime(s_buffer2, sizeof(s_buffer2), clock_is_24h_style() ? "%R" : "%l:%M", diff);
	text_layer_set_text(s_time_diff_layer, s_buffer2);

}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	update_time();
}

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
      GRect(0, 78, bounds.size.w, 48));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorMidnightGreen);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
	
	  // Create the TextLayer with specific bounds
  s_time_diff_layer = text_layer_create(
      GRect(0, 126, bounds.size.w, 42));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_diff_layer, GColorOxfordBlue  );
  text_layer_set_text_color(s_time_diff_layer, GColorWhite);
  text_layer_set_font(s_time_diff_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_time_diff_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_diff_layer));
	
	// Create the TextLayer with specific bounds
  s_date_layer = text_layer_create(
      GRect(0, 0, bounds.size.w, 42));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_date_layer, GColorDarkGreen );
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
	
		// Create the TextLayer with specific bounds
  s_temp_layer = text_layer_create(
      GRect(0, 42, bounds.size.w, 42));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_temp_layer, GColorImperialPurple );
  text_layer_set_text_color(s_temp_layer, GColorWhite);
  text_layer_set_font(s_temp_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_temp_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_temp_layer));
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}