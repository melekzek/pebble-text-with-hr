#include <pebble.h>

// Persistent storage key
#define SETTINGS_KEY 1

// Define our settings struct
typedef struct ClaySettings 
{
  bool color;
  bool HREnabled;
  int slider[5];
  char label[6][20];
} ClaySettings;

// An instance of the struct
static ClaySettings settings;

// Save the settings to persistent storage
static void prv_save_settings() 
{
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
}

// Initialize the default settings
static void prv_default_settings() 
{
  settings.color = false;
  settings.HREnabled = true;
  settings.slider[0] = 120;
  settings.slider[1] = 100;
  settings.slider[2] = 80;
  settings.slider[3] = 60;
  settings.slider[4] = 40;
  strcpy(settings.label[0], "breathless");
  strcpy(settings.label[1], "over excited");
  strcpy(settings.label[2], "excited");
  strcpy(settings.label[3], "calm");
  strcpy(settings.label[4], "sleepy");
  strcpy(settings.label[5], "dead ?");
}

// Read settings from persistent storage
static void prv_load_settings() 
{
  // Load the default settings
  prv_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) 
{
  Tuple *slider = dict_find(iter, MESSAGE_KEY_color);
  if (slider)
    settings.color = slider->value->uint8 > 0;
  slider = dict_find(iter, MESSAGE_KEY_hr);
  if (slider)
    settings.HREnabled = slider->value->uint8 > 0;
  
  slider = dict_find(iter, MESSAGE_KEY_slider0);
  if (slider)
     settings.slider[0] = slider->value->int32;
  slider = dict_find(iter, MESSAGE_KEY_slider1);
  if (slider)
     settings.slider[1] = slider->value->int32;
  slider = dict_find(iter, MESSAGE_KEY_slider2);
  if (slider)
     settings.slider[2] = slider->value->int32;
  slider = dict_find(iter, MESSAGE_KEY_slider3);
  if (slider)
     settings.slider[3] = slider->value->int32;
  slider = dict_find(iter, MESSAGE_KEY_slider4);
  if (slider)
     settings.slider[4] = slider->value->int32;
  
  slider = dict_find(iter, MESSAGE_KEY_input0);
  if (slider)
    strcpy(settings.label[0], slider->value->cstring);
  slider = dict_find(iter, MESSAGE_KEY_input1);
  if (slider)
    strcpy(settings.label[1], slider->value->cstring);
  slider = dict_find(iter, MESSAGE_KEY_input2);
  if (slider)
    strcpy(settings.label[2], slider->value->cstring);
  slider = dict_find(iter, MESSAGE_KEY_input3);
  if (slider)
    strcpy(settings.label[3], slider->value->cstring);
  slider = dict_find(iter, MESSAGE_KEY_input4);
  if (slider)
    strcpy(settings.label[4], slider->value->cstring);
  slider = dict_find(iter, MESSAGE_KEY_input5);
  if (slider)
    strcpy(settings.label[5], slider->value->cstring);
  
//   APP_LOG(APP_LOG_LEVEL_DEBUG, (settings.color ? "white" : "black"));
//   APP_LOG(APP_LOG_LEVEL_DEBUG, (settings.HREnabled ? "hr enabled" : "hr disabled"));
//   for (int i = 0; i < 5; i++)
//     APP_LOG(APP_LOG_LEVEL_DEBUG, "value: %d" ,settings.slider[i]);
//   for (int i = 0; i < 6; i++)
//     APP_LOG(APP_LOG_LEVEL_DEBUG, settings.label[i]);
  prv_save_settings();
}

void prv_init(void) 
{
  // Open AppMessage connection
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(256, 256);
}

static Window *s_window;
static TextLayer *s_text_lines[4];
static int prevMin;
static GFont lg_font;
static GFont sm_font;

static const char* const HOUR[] = {
  "twelve",
  "one",
  "two",
  "three",
  "four",
  "five",
  "six",
  "seven",
  "eight",
  "nine",
  "ten",
  "eleven"
};

static void update_time(struct tm* t) 
{
  /*
  int step_count = 2345;
  if (show_steps) {
    time_t start = time_start_of_today();
    time_t end = time(NULL);
    // Check the metric has data available for today
    HealthServiceAccessibilityMask mask = health_service_metric_accessible(metric, start, end);
    if(mask & HealthServiceAccessibilityMaskAvailable) {
      step_count = (int) health_service_sum_today(metric);
      // Data is available!
      APP_LOG(APP_LOG_LEVEL_INFO, "Steps today: %d", step_count);
    } else {
      // No data recorded yet today
      APP_LOG(APP_LOG_LEVEL_ERROR, "Data unavailable!");
    }
  }
  */
  
  const int min = (((t->tm_min + 2) % 60) / 5) * 5;
  // early exit to save battery
  // TODO: do I need to check hour ?
  if (prevMin == min)
    return;
  prevMin = min;
  bool preText = false;
  char* postText = "                                ";
  if (prevMin >= 55)
    strcpy(postText, "almost ");
  else if (prevMin >= 45)
    strcpy(postText, "quarter to ");
  else if (prevMin >= 40)
  {
    preText = true;
    strcpy(postText, "quarter to ");
  }
  else if (prevMin >= 30)
    strcpy(postText, "half past ");
  else if (prevMin >= 25)
  {
    preText = true;
    strcpy(postText, "half past ");
  }
  else if (prevMin >= 15)
    strcpy(postText, "quarter past ");
  else if (prevMin >= 10)
  {
    preText = true;
    strcpy(postText, "quarter past ");
  }
  else
    strcpy(postText, "roughly");
    
  text_layer_set_text(s_text_lines[0], (preText ? "it is almost" : "it is"));
  text_layer_set_text(s_text_lines[1], postText);
  const int hour = t->tm_hour + (prevMin >= 40 || t->tm_min > 57 ? 1 : 0);
  text_layer_set_text(s_text_lines[2], HOUR[hour%12]);
  
  #if PBL_API_EXISTS(health_service_peek_current_value)
      char* hrText = "                                                               ";
      if (settings.HREnabled)
      {
        const HealthValue bpm = health_service_peek_current_value(HealthMetricHeartRateBPM);
        int i = 0;
        for (; i < 5; i++)
        {
          if (bpm > settings.slider[i])
          {
            strcpy(hrText, settings.label[i]);
            break;
          }
        }
        if (i == 5)
          strcpy(hrText, settings.label[5]);
      }
      //static char hr[] = "000"; 
      //snprintf(hr, sizeof(hr), "%lu", (unsigned long)bpm);
      //strcat(hrText, " - ");
      //strcat(hrText, hr);
      text_layer_set_text(s_text_lines[3], hrText);
  #endif
}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) 
{
  update_time(tick_time);
}

static TextLayer* setupTextLayer(const GRect* rect, const GFont* font)
{
  TextLayer* layer = text_layer_create(*rect);
	text_layer_set_text_color(layer, settings.color ? GColorBlack : GColorWhite);
  text_layer_set_background_color(layer, GColorClear);
  text_layer_set_text_alignment(layer, GTextAlignmentLeft);
  text_layer_set_font(layer, *font);
  return layer;
}

static void load(Window* window) 
{
  Layer *window_layer = window_get_root_layer(window);
  const GRect bounds = layer_get_bounds(window_layer);
	
  const int totalHeight = 60;
  const int fontHeight = 24;
  lg_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BALOO_22));
  sm_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BALOO_14));
  
  const int left = bounds.origin.x + 6;
  window_set_background_color(s_window, settings.color ? GColorWhite : GColorBlack);
  const int heightVar = bounds.size.h/2 - totalHeight;
  // Create a text layer and set the text
  const GRect topTextRect = GRect(left, heightVar, bounds.size.w, fontHeight + 2);
	s_text_lines[0] = setupTextLayer(&topTextRect, &lg_font);
  
  // Create an "almost" layer and set the text
  const GRect almostTextRect = GRect(left, heightVar + fontHeight * 1.2, bounds.size.w, fontHeight + 2);
	s_text_lines[1] = setupTextLayer(&almostTextRect, &lg_font);
  
  // Create a hour layer and set the text
  const GRect hourTextRect = GRect(left, heightVar + fontHeight * 2.4, bounds.size.w, fontHeight + 2);
	s_text_lines[2] = setupTextLayer(&hourTextRect, &lg_font);
  
    // Create a hour layer and set the text
  const GRect hrTextRect = GRect(left, bounds.size.h * 0.8, bounds.size.w, fontHeight);
	s_text_lines[3] = setupTextLayer(&hrTextRect, &sm_font);
  
  text_layer_set_text(s_text_lines[1], "vague text");
  text_layer_set_text(s_text_lines[2], "with hr");
  
	// Add the text layer to the window
  for (int i = 0; i < 4; i++)
	  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_text_lines[i]));
  
  // Enable text flow and paging on the text layer, with a slight inset of 10, for round screens
  //text_layer_enable_screen_text_flow_and_paging(s_text_layer, 10);
  
  prevMin = -1;
	// App Logging!
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
}

static void unload(Window* window)
{
  	// Destroy the text layer
  for (int i = 0; i < 4; i++)
	  text_layer_destroy(s_text_lines[i]);
  
  fonts_unload_custom_font(lg_font);
  fonts_unload_custom_font(sm_font);
}

static void init(void)
{
  prv_load_settings();
  tick_timer_service_subscribe(MINUTE_UNIT, &handle_minute_tick);
  // Create a window and get information about the window
	s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = load,
    .unload = unload,
  });
  window_stack_push(s_window, false /*animated*/);

  prv_init();
//   UnobstructedAreaHandlers handlers = {
//     .will_change = prv_unobstructed_did_change,
//   };
//   unobstructed_area_service_subscribe(handlers, NULL);
}

static void deinit(void)
{ 
  // Destroy the window
	window_destroy(s_window);
  tick_timer_service_unsubscribe();
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
