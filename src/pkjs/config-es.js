module.exports = [
	{
		"type": "heading",
		"defaultValue": "Ajustes"
	},
	{
		"type": "text",
		"defaultValue": "Esta es la página de ajustes para la app Breathe. Estás usando la versión 1.1 de la app.",
	},
	{
		"type": "section",
		"items": [
			{
				"type": "heading",
				"defaultValue": "Colores"
			},
			{
				"type": "color",
				"messageKey": "backgroundColor",
				"defaultValue": "000000",
				"label": "Color de fondo",
				"sunlight": true,
				"layout": "BLACK_WHITE"
			},
			{
				"type": "color",
				"messageKey": "circleColor",
				"defaultValue": "00AA55",
				"label": "Color del círculo",
				"sunlight": true,
				"capabilities": ["COLOR"]
			}
		]
	},
	{
		"type": "section",
		"items": [
			{
				"type": "heading",
				"defaultValue": "En la app"
			},
			{
				"type": "toggle",
				"messageKey": "rememberDuration",
				"defaultValue": false,
				"label": "¿Recordar la duración anterior y usarla cuando se lance la app?",
			},
			{
				"type": "toggle",
				"messageKey": "vibrationEnabled",
				"defaultValue": true,
				"label": "¿Activar las vibraciones durante la respiración?",
			},
			{
				"type": "select",
				"messageKey": "vibrationType",
				"defaultValue": "0",
				"label": "Tipo de vibración",
				"description": "Sacudidas es como el Apple Watch; Toques es un doble toque.",
				"options": [
					{
						"label": "Sacudidas - inhalación solamente",
						"value": "0"
					},
					{
						"label": "Sacudidas - inhalación y exhalación",
						"value": "2"
					},
					{ 
						"label": "Toques", 
						"value": "1"
					}
				]
			},
			{
				"type": "toggle",
				"capabilities": [
					"NOT_PLATFORM_APLITE", 
					"NOT_PLATFORM_BASALT",
					"NOT_PLATFORM_CHALK"
					],
				"messageKey": "heartRateVariation",
				"defaultValue": false,
				"label": "¿Escoger la velocidad de respiraciones según el ritmo cardiaco?",
				"description": "NOTA: Esto requiere un reloj con un monitor de ritmo cardiaco."
			},
			{
				"type": "slider",
				"messageKey": "breathsPerMinute",
				"defaultValue": 7,
				"label": "Respiraciones por minuto",
				"min": 4,
				"max": 10,
				"step": 1
			}
		]
	},
	{
		"type": "section",
		"capabilities": ["HEALTH"],
		"items": [
			{
				"type": "heading",
				"defaultValue": "Salud"
			},
			{
				"type": "select",
				"messageKey": "displayText",
				"defaultValue": "1",
				"label": "Mostrar...",
				"description": "Esto determina lo que está mostrado en el menu principal. NOTA: El ritmo cardiaco requiere un reloj con un monitor de ritmo cardiaco.",
				"options": [
					{
						"label": "Nada",
						"value": "0"
					},
					{
						"label": "Saludo",
						"value": "1"
					},
					{ 
						"label": "Pasos hoy", 
						"value": "2" 
					},
					{ 
						"label": "Ritmo cardiaco",
						"value": "3" 
					}
				]
			},
		]
	},
	{
		"type": "section",
		"items": [
			{
				"type": "heading",
			 	"defaultValue": "Recordatorios"
			},
			{
				"type": "select",
				"messageKey": "reminderHours",
				"defaultValue": "4",
				"label": "Recuérdame de respirar...",
				"description": "El app te recordará a este intervalo diariamente.",
				"options": [
					{ 
						"label": "Cada hora", 
						"value": "1" 
					},
					{ 
						"label": "Cada 2 horas", 
						"value": "2" 
					},
					{ 
						"label": "Cada 3 horas",
						"value": "3" 
					},
					{ 
						"label": "Cada 4 horas",
						"value": "4" 
					},
					{ 
						"label": "Cada 6 horas",
						"value": "6" 
					},
					{ 
						"label": "No me recuerda nunca",
						"value": "0" 
					}
				]
			},
			{
				"type": "slider",
				"messageKey": "reminderHoursStart",
				"defaultValue": 8,
				"label": "¿Cuando debería empezar de recordarte la app?",
				"description": "De esta hora (es decir, 8 = las 8 de la mañana) hasta 12 horas trás, la app de recordará de respirar.",
				"min": 6,
				"max": 10,
				"step": 1
			}
		]
	},
	{
		"type": "text",
		"defaultValue": "<em><center>Muchas gracias a los probadores beta: <br>Paula&nbsp;Bosca, Nikita&nbsp;Cheng, Ayush&nbsp;Gupta, Ellen&nbsp;Huang, Yvonne&nbsp;Tan, David&nbsp;Voicu, /u/dryingsocks, y /u/PiwwowPants</center></em>",
	},
	{
		"type": "submit",
		"defaultValue": "Guardar mis ajustes"
	}
];