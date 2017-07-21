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
    "label": "white on black",
    "messageKey": "color",
    "defaultValue": true
  },
  {
    "type": "toggle",
    "label": "double tap for digital",
    "messageKey": "digital",
    "defaultValue": true
  },
   {
    "type": "section",
    "items": [
  {
  "type": "heading",
  "defaultValue": "Text settings",
  "size": 1
  },
  {
    "type": "toggle",
    "messageKey": "hr",
    "label": "Enable hearthrate",
    "defaultValue": true
  },
  {
    "type": "toggle",
    "messageKey": "itis",
    "label": "Display 'it is'",
    "defaultValue": true
  },
  {
    "type": "toggle",
    "messageKey": "youare",
    "label": "Display 'youare'",
    "defaultValue": true
  },
  {
    "type": "toggle",
    "messageKey": "align",
    "label": "Align text left",
    "defaultValue": true
  },
  {
    "type": "toggle",
    "messageKey": "roughly",
    "label": "Display 'roughly'",
    "defaultValue": true
  },
    ]
  },
  {
    "type": "section",
    "items": [
  {
  "type": "heading",
  "defaultValue": "HR Settings",
  "size": 1
  },
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
      {
        "type": "input",
        "messageKey": "input5",
        "label": "if hr is lower than all, describe as",
        "defaultValue": "flatline"
    },
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];
