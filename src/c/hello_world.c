#include <pebble.h>

// Persistent storage key
#define SETTINGS_KEY 1

// Define our settings struct
typedef struct ClaySettings 
{
  bool color;
  bool HREnabled;
  bool itis;
  bool youare;
  bool align;
  bool roughly;
  bool digital;
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
  settings.color = true;
  settings.HREnabled = true;
  settings.itis = true;
  settings.youare = true;
  settings.align = true;
  settings.roughly = true;
  settings.digital = true;
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
  strcpy(settings.label[5], "flatline");
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
  
  slider = dict_find(iter, MESSAGE_KEY_itis);
  if (slider)
    settings.itis = slider->value->uint8 > 0;

  slider = dict_find(iter, MESSAGE_KEY_youare);
  if (slider)
    settings.youare = slider->value->uint8 > 0;

  slider = dict_find(iter, MESSAGE_KEY_align);
  if (slider)
    settings.align = slider->value->uint8 > 0;
  
  slider = dict_find(iter, MESSAGE_KEY_roughly);
  if (slider)
    settings.roughly = slider->value->uint8 > 0;
  
  slider = dict_find(iter, MESSAGE_KEY_digital);
  if (slider)
    settings.digital = slider->value->uint8 > 0;

  const uint32_t sliderKeys[] = 
  {
    MESSAGE_KEY_slider0,
    MESSAGE_KEY_slider1,
    MESSAGE_KEY_slider2,
    MESSAGE_KEY_slider3,
    MESSAGE_KEY_slider4
  };
  for (int i = 0; i < 5; i++)
  {
    slider = dict_find(iter, sliderKeys[i]);
    if (slider)
      settings.slider[i] = slider->value->int32;
  }
  
  const uint32_t labelKeys[] = 
  {
    MESSAGE_KEY_input0,
    MESSAGE_KEY_input1,
    MESSAGE_KEY_input2,
    MESSAGE_KEY_input3,
    MESSAGE_KEY_input4,
    MESSAGE_KEY_input5
  };
  for (int i = 0; i < 6; i++)
  {
    slider = dict_find(iter, labelKeys[i]);
    if (slider)
      strcpy(settings.label[i], slider->value->cstring);
  }
  
  prv_save_settings();
}

void prv_init(void) 
{
  // Open AppMessage connection
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(256, 256);
}


// variables -----------------------------
static Window *s_window;
static TextLayer *s_text_lines[7];
static int prevMin;
static GFont lg_font[3];
static GFont sm_font;
static GFont huge_font;
static time_t lastTap;
static bool realTime;
static AppTimer *appTimer;
static char digitalText[10];

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

enum textLine
{
  ItIs = 0,
  CenterHour,
  BottomHour,
  YouAre,
  HrText,
  DigitalHour,
  DigitalHr
};

// -----------------------------

static void setText(uint line, const char foo[])
{
  text_layer_set_text(s_text_lines[line], foo);
}

static void redraw_time(int tm_hour, int tm_min) 
{
  if (realTime)
  {
    for (int i = 0; i < 3; i++)
      text_layer_set_text(s_text_lines[i], "");
    
    char hour_text[3];
    char min_text[3];
    snprintf(hour_text, sizeof(hour_text), "%02u", tm_hour);
    snprintf(min_text, sizeof(min_text), "%02u", tm_min);
    strcpy(digitalText, hour_text);
    strcat(digitalText, ":");
    strcat(digitalText, min_text);
    setText(DigitalHour, digitalText);
  }
  else
  {
    const int min = (((tm_min + 2) % 60) / 5) * 5;
    // early exit to save battery
    // TODO: do I need to check hour ?
    if (prevMin == min)
      return;
    prevMin = min;
    bool preText = false;
    bool shiftHour = false;
    char* postText = "                                ";
    if (prevMin >= 55)
    {
      strcpy(postText, "almost");
    }
    else if (prevMin >= 45)
    {
      strcpy(postText, "quarter to");
    }
    else if (prevMin >= 40)
    {
      preText = true;
      strcpy(postText, "quarter to");
    }
    else if (prevMin >= 30)
    {
      strcpy(postText, "half past");
    }
    else if (prevMin >= 25)
    {
      preText = true;
      strcpy(postText, "half past");
    }
    else if (prevMin >= 15)
    {
      strcpy(postText, "quarter past");
    }
    else if (prevMin >= 10)
    {
      preText = true;
      strcpy(postText, "quarter past");
    }
    else
    {
      if (!settings.roughly)
        shiftHour = false; // true;
      else
        strcpy(postText, "roughly");
    }
    
    char* topline = "                 ";
    strcpy(topline, (settings.itis ? "it is " : ""));
    strcat(topline, (preText ? "almost" : ""));
    setText(ItIs, topline);
    if (!shiftHour)
    {
      setText(CenterHour, postText);
    }
    else
    {
      setText(BottomHour, "");
    }
    
    const int hour = tm_hour + (prevMin >= 40 || tm_min > 57 ? 1 : 0);
    setText((shiftHour) ? CenterHour : BottomHour, HOUR[hour%12]);
    setText(DigitalHour, "");
  }
  
  #if PBL_API_EXISTS(health_service_peek_current_value)
      char* hrText = "                                                               ";
      if (settings.HREnabled)
      {
        const HealthValue bpm = health_service_peek_current_value(HealthMetricHeartRateBPM);
        if (realTime)
        {
          char hr_text[10];
          const uint hr = bpm;
          snprintf(hr_text, sizeof(hr_text), "%u", hr);
          strcpy(hrText, hr_text);
          strcat(hrText, " bpm");
          setText(YouAre, "");
          setText(HrText, "");
          setText(DigitalHr, hrText);
        }
        else
        {
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
          
          setText(YouAre, (settings.youare) ? "you are" : "");
          setText(HrText, hrText);
          setText(DigitalHr, "");
        }
      }
      else
      {
        setText(YouAre, "");
        setText(HrText, "");
        setText(DigitalHr, "");
      }
  #endif
}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) 
{
  redraw_time(tick_time->tm_hour, tick_time->tm_min);
}

static TextLayer* setupTextLayer(const GRect* rect, const GFont* font)
{
  TextLayer* layer = text_layer_create(*rect);
	text_layer_set_text_color(layer, settings.color ? GColorBlack : GColorWhite);
  text_layer_set_background_color(layer, GColorClear);
  text_layer_set_text_alignment(layer, (settings.align) ? GTextAlignmentLeft : GTextAlignmentRight);
  text_layer_set_font(layer, *font);
  return layer;
}

static void load(Window* window) 
{
  Layer *window_layer = window_get_root_layer(window);
  const GRect bounds = layer_get_bounds(window_layer);
	
  const int totalHeight = 60;
  const int fontHeight = 24;
  lg_font[2] = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BALOO_26));
  lg_font[1] = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BALOO_22));
  lg_font[0] = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BALOO_20));  

  sm_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BALOO_14));
  huge_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BALOO_52));
  
  const int left = bounds.origin.x + 3;
  const int width = bounds.size.w - 6;
  window_set_background_color(s_window, settings.color ? GColorWhite : GColorBlack);
  const int heightVar = bounds.size.h/2 - totalHeight;
  // Create a text layer and set the text
  const GRect topTextRect = GRect(left, heightVar, width, fontHeight + 2);
	s_text_lines[0] = setupTextLayer(&topTextRect, &lg_font[0]);
  
  // Create an "almost" layer and set the text
  const GRect almostTextRect = GRect(left, heightVar + fontHeight * 1.1, width, fontHeight + 2);
	s_text_lines[1] = setupTextLayer(&almostTextRect, &lg_font[1]);
  
  // Create a hour layer and set the text
  const GRect hourTextRect = GRect(left, heightVar + fontHeight * 2.2, width, fontHeight + 2 + 4);
	s_text_lines[2] = setupTextLayer(&hourTextRect, &lg_font[2]);
  
    // Create a hr layer and set the text
  const GRect hrTextRect = GRect(left, bounds.size.h * 0.8 - 14 , width, fontHeight);
	s_text_lines[3] = setupTextLayer(&hrTextRect, &sm_font);
  
  const GRect youareTextRect = GRect(left, bounds.size.h * 0.8, width, fontHeight);
	s_text_lines[4] = setupTextLayer(&youareTextRect, &sm_font);
  
  const GRect digitTextRect = GRect(left, bounds.size.h * 0.2, width, 70);
	s_text_lines[5] = setupTextLayer(&digitTextRect, &huge_font);
  
  const GRect digitHrRect = GRect(left, bounds.size.h * 0.8 - 14, width, fontHeight * 2.2);
	s_text_lines[6] = setupTextLayer(&digitHrRect, &lg_font[2]);
  
  text_layer_set_text(s_text_lines[1], "vague text");
  text_layer_set_text(s_text_lines[2], "with hr");
  
	// Add the text layer to the window
  for (int i = 0; i < 7; i++)
	  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_text_lines[i]));
  
  prevMin = -1;
  lastTap = 0;
  realTime = false;
  appTimer = NULL;
}

static void unload(Window* window)
{
  	// Destroy the text layer
  for (int i = 0; i < 7; i++)
	  text_layer_destroy(s_text_lines[i]);
  
  for (int i = 0; i < 3; i++)
    fonts_unload_custom_font(lg_font[i]);
  fonts_unload_custom_font(sm_font);
  fonts_unload_custom_font(huge_font);
}

static void timerCallback(void *data)
{
  appTimer = NULL;
  realTime = false;
  lastTap = 0;
  prevMin = -1;
  const time_t currTap = time(NULL);
  struct tm* currTm = localtime(&currTap);
  redraw_time(currTm->tm_hour, currTm->tm_min);
}

static void accel_tap_handler(AccelAxisType axis, int32_t direction)
{
  const time_t currTap = time(NULL);
  const time_t secPassed = currTap - lastTap;    
  lastTap = currTap;
  if (secPassed < 2 && settings.digital)
  {
    if (appTimer)
    {
      app_timer_cancel(appTimer);
      appTimer = NULL;
    }
    realTime = !realTime;
    lastTap = 0;
    prevMin = -1;
    struct tm* currTm = localtime(&currTap);
    redraw_time(currTm->tm_hour, currTm->tm_min);
    
    // start timer to revert back to text
    if (realTime)
    {
      appTimer = app_timer_register(5000, timerCallback, NULL);
    }
  }
}

static void init(void)
{
  prv_load_settings();
  tick_timer_service_subscribe(MINUTE_UNIT, &handle_minute_tick);
  accel_tap_service_subscribe(accel_tap_handler);
  
  // Create a window and get information about the window
	s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = load,
    .unload = unload,
  });
  window_stack_push(s_window, false /*animated*/);

  prv_init();
}

static void deinit(void)
{ 
  // Destroy the window
	window_destroy(s_window);
  accel_tap_service_unsubscribe();
  tick_timer_service_unsubscribe();
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
