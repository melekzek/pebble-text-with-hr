module.exports = [
  {
    "type": "heading",
    "defaultValue": "App Configuration"
  },
  {
    "type": "text",
    "defaultValue": "Vague text based watchface with hearthrate"
  },
  {
    "type": "toggle",
    "label": "white on black or black on white",
    "messageKey": "color",
    "defaultValue": false
  },
  {
    "type": "toggle",
    "messageKey": "hr",
    "label": "Enable hearthrate",
    "defaultValue": true
  },
  {
    "type": "section",
    "items": [
      {
        "type": "slider",
        "messageKey": "slider0",
        "label": "For HR higher than",
        "defaultValue": 120,
        "min": 40,
        "max": 180,
        "step": 10
      },
      {
        "type": "input",
        "messageKey": "input0",
        "label": "describe as",
        "defaultValue": "breathless"
      },
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "slider",
        "messageKey": "slider1",
        "label": "For HR higher than",
        "defaultValue": 100,
        "min": 40,
        "max": 180,
        "step": 10
      },
      {
        "type": "input",
        "messageKey": "input1",
        "label": "describe as",
        "defaultValue": "over excited"
      },
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "slider",
        "messageKey": "slider2",
        "label": "For HR higher than",        
        "defaultValue": 80,
        "min": 40,
        "max": 180,
        "step": 10
      },
      {
        "type": "input",
        "messageKey": "input2",
        "label": "describe as",
        "defaultValue": "excited"
      },
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "slider",
        "messageKey": "slider3",
        "label": "For HR higher than",      
        "defaultValue": 60,
        "min": 40,
        "max": 180,
        "step": 10
      },
      {
        "type": "input",
        "messageKey": "input3",
        "label": "describe as",
        "defaultValue": "calm"
      },
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "slider",
        "messageKey": "slider4",
        "label": "For HR higher than",        
        "defaultValue": 40,
        "min": 40,
        "max": 180,
        "step": 10
      },
      {
        "type": "input",
        "messageKey": "input4",
        "label": "describe as",
        "defaultValue": "sleepy"
      },
    ]
  },
  {
        "type": "input",
        "messageKey": "input5",
        "label": "describe as",
        "defaultValue": "dead ?"
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];
