# Breathe

![](/screenshots/Aplite_Marketing_v8.png)

![](/screenshots/index_blue.png)
![](/screenshots/index_red.png)
![](/screenshots/index_green.png)
![](/screenshots/index_yellow.png)


### Featured on the Pebble App Store from December 2016 to January 2017!
 [![Available on the Pebble App Store](/screenshots/badge.png)][appstore]

## Description:

### Breathe with a beautiful interface.
- A simple circle animation expands and contracts to help you visualize and focus on breathing.
- Gentle vibrations allow you to use the app even if you're not looking at the screen, and this vibration can be set to one of three types: a fancy vibration pattern on inhale and exhale, a fancy vibration on inhale only, or a simple double tap. And if you don't like vibrations, you can just turn'em off.
- The background colour and circle colour are completely customizable. Don't like green and black? Change it to any of the 64 supported colours allowed by Pebble's screen!

### Breathe at your own speed.
- The breathing speed can be changed from 4 to 10 breaths per minute, perfect for when you want to calm down just a bit or quite a lot.
- The duration of the breathing as shown on the main menu can be optionally set to remember the duration of the last breathing session. Or it can just start at 1 minute, the default, each time. Your choice!
- For heart-rate enabled smartwatches, the number of breaths taken per minute can be set dynamically based on the current heart rate, and the current heart rate is shown at the top of the screen during the breathing session. Thanks to [JohnEdwa](https://github.com/JohnEdwa)!

### See your health stats.
- The main screen displays how many minutes you've breathed today using this app. It also displays the number of steps taken today, a greeting, your current heart rate, or nothing at all depending on your settings.

### Be reminded to take a moment and breathe.
- A reminder can be set (every 1, 2, 3, 4, or 6 hours) as a prompt to breathe. You can change when these reminders start (if you don't like waking up early, you can set them to start at 10 AM!). This reminder can also be snoozed for any increment of 10 minutes up to 50 minutes, for when you're just a bit busy at the moment.
- The App Glance shows how many minutes you've breathed in the last session. This can be set to show the current daily total, or be disabled completely. Thanks to [PiwwowPants](https://github.com/PiwwowPants)!

### Get motivation with achievements and stats.
- Breathe and get achievements: some for how long you've breathed, others for how consistent you've been, and others for just customizing your settings. Get badges and see your achievements with a long press of the down button!
- See stats about how long you've spent with this app, as well as your longest streak.

### Supports multiple languages.
- Don't speak English? This app supports English, French, Spanish, and German (text in app, settings page all correspond to the watch's chosen language). Thanks to [noiob](https://github.com/noiob) for the German translation!

## Some more screenshots:

*Pebble Classic/Steel/2, Pebble Time (Steel), Pebble Time Round, Pebble Time 2*

![](/screenshots/aplite_final.gif)
![](/screenshots/basalt_final.gif)
![](/screenshots/chalk_final.gif)
![](/screenshots/emery_final.gif)

[appstore]: https://apps.getpebble.com/en_US/application/583dc06f00355abe66000862

## Build and run

### Cloudpebble

You can clone this repository with its URL and import to Cloudpebble. It should be able to build and run when pressing the green play button.

### Command line tools

You'll need to have the Pebble Command Line Tools installed.

After that, from the root of the repository, ```pebble build``` to build the .pbw file.

To install on a watch, enable the Developer connection from the Pebble mobile app, then ```pebble install --phone <IP_ADDRESS>```, where the <IP_ADDRESS> is the one that appears beneath the developer connection section in the ombile app.

To install on the emulator, ```pebble install --emulator <HARDWARE>```, where <HARDWARE> is any one of the codenames used for Pebble hardware, i.e. Aplite, Basalt, Chalk, Diorite, or Emery.
