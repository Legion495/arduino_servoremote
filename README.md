# Arduino Servo Remote

A simple Arduino web interface for a school project. Uses [NodeJS](https://nodejs.org/) & [GulpJS](http://gulpjs.com/) to build and gzip the assets because the Arduino Webserver only supports single files up to 40kb.

## cloning & building

Gulp is set to automatically build a demo in `./dist/` and Arduino-ready files in `./arduino/server/data/` post-install:
`git clone https://github.com/PoweredBy115/arduino_servoremote.git && cd arduino_servoremote && npm install`

`gulp dev` is set to watch the `./gulp/` subfolders and automatically updates the build from them.

## short doc

The web app includes [gyro.js](https://github.com/tomgco/gyro.js) and [jquery](https://github.com/jquery/jquery).

`http://arduino_ip/` serves a simple web app which sends POST requests to the Arduino to control two servomotors.
An exemplary POST sent by the app looks like this (sets both left & right to 50%): `/motor?l=0.5&r=0.5"`

The web app is designed to run on smartphones and calculates the speed for both continuous tracks depending on the tilt of the gyroscope. Gyroscope data is polled and sent 10 times per second. Average server response times we measured were ~50ms, 10 POSTs per second should be fine.
