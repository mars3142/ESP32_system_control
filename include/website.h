#pragma once

#include <Arduino.h>

const char index_html[] PROGMEM = R"(
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <link
      rel="stylesheet"
      href="https://cdn.jsdelivr.net/npm/bulma@0.9.4/css/bulma.min.css"
    />

    <title>Train Control</title>

    <style>
      input.slider {
        border: solid red 1px;
        padding: 0;
        -webkit-appearance: auto;
      }

      .field.has-addons .control {
        width: 50%;
      }

      .field .control {
        text-align: center;
      }

      .field .control canvas {
        margin: 20px;
        text-align: center;
      }

      .field .control button {
        width: 100%;
      }
    </style>
  </head>

  <body>
    <section class="section">
      <div class="container">
        <div class="card p-2 mb-6">
          <div class="field has-addons">
            <p class="control">
              <button class="button" id="day" onclick="sendDNState('DAY');">
                <span>Tag</span>
              </button>
            </p>
            <p class="control">
              <button class="button" id="night" onclick="sendDNState('NIGHT');">
                <span>Nacht</span>
              </button>
            </p>
          </div>
        </div>
        <hr />
        <h2 class="title is-2">Einstellungen</h2>
        <form class="" onsubmit="sendData(); return false;">
          <div class="columns">
            <div class="column is-3">
              <div class="box">
                <h3 class="title is-3">Allgemein</h3>

                <div class="field">
                  <label class="label">LED Count</label>
                  <div class="control">
                    <input
                      class="input"
                      type="number"
                      name="ledCount"
                      id="ledCount"
                      value="0"
                    />
                  </div>
                </div>
                <div class="field">
                  <label class="label"
                    >Helligkeit (<output id="brightnessValue"></output>)</label
                  >
                  <div class="control">
                    <input
                      class="input slider"
                      type="range"
                      name="brightness"
                      id="brightness"
                      value="0"
                      min="0"
                      max="255"
                      list="brightnessValues"
                    />
                  </div>
                </div>

                <div class="field">
                  <label class="label"
                    >Übergangszeit (<output id="delayValue"></output>
                    sec)</label
                  >
                  <div class="control">
                    <input
                      class="input slider"
                      type="range"
                      name="delay"
                      id="delay"
                      value="0"
                      min="1"
                      max="10"
                      list="delayValues"
                    />
                  </div>
                </div>

                <datalist id="brightnessValues">
                  <option value="0"></option>
                  <option value="64"></option>
                  <option value="128"></option>
                  <option value="192"></option>
                  <option value="255"></option>
                </datalist>

                <datalist id="delayValues">
                  <option value="1"></option>
                  <option value="2"></option>
                  <option value="3"></option>
                  <option value="4"></option>
                  <option value="5"></option>
                  <option value="6"></option>
                  <option value="7"></option>
                  <option value="8"></option>
                  <option value="9"></option>
                  <option value="10"></option>
                </datalist>
              </div>
            </div>
            <div class="column">
              <div class="box">
                <h3 class="title is-3">Tag</h3>

                <div class="field">
                  <label class="label">Farbe</label>
                  <div class="control">
                    <input
                      class="input"
                      type="color"
                      name="colorDay"
                      id="colorDay"
                      value="#ff4dcf"
                      disabled
                    />
                    <canvas class="colorWheel" id="colorWheelDay"></canvas>
                  </div>
                </div>
              </div>
            </div>

            <div class="column">
              <div class="box">
                <h3 class="title is-3">Nacht</h3>
                <div class="field">
                  <label class="label">Farbe</label>
                  <div class="control">
                    <input
                      class="input"
                      type="color"
                      name="colorNight"
                      value="#5fff5f"
                      id="colorNight"
                      disabled
                    />
                    <canvas class="colorWheel" id="colorWheelNight"></canvas>
                  </div>
                </div>
              </div>
            </div>
          </div>
          <button class="button is-primary" id="submit">Speichern</button>
        </form>
      </div>
    </section>

    <script>
      let dnState = "";

      const brightness = document.querySelector("#brightnessValue");
      const brightnessSlider = document.querySelector("#brightness");
      brightness.textContent = brightnessSlider.value;
      brightnessSlider.addEventListener("input", (event) => {
        brightness.textContent = event.target.value;
      });

      const delay = document.querySelector("#delayValue");
      const delaySlider = document.querySelector("#delay");
      delay.textContent = delaySlider.value;
      delaySlider.addEventListener("input", (event) => {
        delay.textContent = event.target.value;
      });

      function ws_connect() {
        ws = new WebSocket("ws://eisenbahn.local/state");

        ws.addEventListener("error", (event) => {
          console.log("Error: " + JSON.stringify(event));
          ws.close();
        });

        ws.addEventListener("open", (event) => {
          console.log("Connected to websocket");
          ws.send("{ 'command': 'init_data', 'auth': 'eisenbahn' }");
        });

        ws.addEventListener("close", (event) => {
          console.log("Closed websocket connection");
          setTimeout(ws_connect, 1000);
        });

        ws.addEventListener("message", (event) => {
          console.log("Message from server: ", event.data);
          try {
            const data = JSON.parse(event.data);
            setVal("colorDay", data.colorDay || "#001122");
            setVal("colorNight", data.colorNight || "#334455");
            setVal("ledCount", data.ledCount || 500);
            setVal("brightness", data.brightness || 192);
            setVal("brightnessValue", data.brightness || 192);
            setVal("delay", data.delay || 5);
            setVal("delayValue", data.delay || 5);
            setDayNight(data.state || "day");
          } catch (e) {
            console.error(e);
          }
        });
      }

      function getVal(id) {
        return find(id).value;
      }

      function setVal(id, value) {
        find(id).value = value;
      }

      function find(id) {
        return document.getElementById(id);
      }

      function setDayNight(state) {
        find("day").classList.remove("is-warning");
        find("night").classList.remove("is-black");
        if (state.toUpperCase() === "NIGHT") {
          dnState = "NIGHT";
          find("night").classList.add("is-black");
          return;
        }
        dnState = "DAY";
        find("day").classList.add("is-warning");
      }

      function sendData() {
        const data = {
          ledCount: parseInt(getVal("ledCount")),
          colorDay: getVal("colorDay"),
          colorNight: getVal("colorNight"),
          brightness: parseInt(getVal("brightness")),
          delay: parseInt(getVal("delay")),
          state: dnState,
        };
        console.log("Sending data: " + JSON.stringify(data));
        ws.send(data);
      }

      function sendDNState(state) {
        setDayNight(state);
        sendData();
      }

      ws_connect();

      // color wheels
      let radius = 120;
      initWheels();

      function initWheels() {
        initWheel("colorWheelDay", "colorDay");
        initWheel("colorWheelNight", "colorNight");
      }

      function initWheel(id, outputId) {
        let canvas = find(id);
        let ctx = canvas.getContext("2d");

        canvas.removeEventListener("click", (e) =>
          onWheelClick(e, canvas, outputId)
        );
        canvas.addEventListener("click", (e) =>
          onWheelClick(e, canvas, outputId)
        );
        canvas.width = radius * 2;
        canvas.height = radius * 2;

        find(outputId).removeEventListener("change", (e) =>
          drawCircle(ctx, e.target.value)
        );
        find(outputId).addEventListener("change", (e) =>
          drawCircle(ctx, e.target.value)
        );

        drawCircle(ctx, find(outputId).value);

        function onWheelClick(e, canvas, outputId) {
          let ctx = canvas.getContext("2d");
          ctx.clearRect(0, 0, canvas.width, canvas.height);

          drawCircle(ctx);
          let x = e.offsetX;
          let y = e.offsetY;

          drawPickerHandle(ctx, x, y);

          let [r, phi] = xy2polar(x - radius, y - radius);
          let deg = rad2deg(phi);
          let hue = deg;
          let saturation = r / radius;
          let [red, green, blue] = hsv2rgb(hue, saturation, 1);
          find(outputId).value = rgbToHex(red, green, blue);
        }
      }

      function drawPickerHandle(ctx, x, y) {
        ctx.beginPath();
        ctx.fillStyle = "white";
        ctx.arc(x, y, 10, 0, 360);
        ctx.closePath();
        ctx.fill();

        ctx.beginPath();
        ctx.fillStyle = "black";
        ctx.arc(x, y, 8, 0, 360);
        ctx.closePath();
        ctx.fill();
      }

      function drawCircle(ctx, currentColorHex) {
        let image = ctx.createImageData(2 * radius, 2 * radius);
        let data = image.data;
        let markerCurentColorPos;

        for (let x = -radius; x < radius; x++) {
          for (let y = -radius; y < radius; y++) {
            let [r, phi] = xy2polar(x, y);

            if (r > radius) {
              // skip all (x,y) coordinates that are outside of the circle
              continue;
            }

            let deg = rad2deg(phi);

            // Figure out the starting index of this pixel in the image data array.
            let rowLength = 2 * radius;
            let adjustedX = x + radius; // convert x from [-50, 50] to [0, 100] (the coordinates of the image data array)
            let adjustedY = y + radius; // convert y from [-50, 50] to [0, 100] (the coordinates of the image data array)
            let pixelWidth = 4; // each pixel requires 4 slots in the data array
            let index = (adjustedX + adjustedY * rowLength) * pixelWidth;

            let hue = deg;
            let saturation = r / radius;
            let value = 1.0;

            let [red, green, blue] = hsv2rgb(hue, saturation, value);
            let alpha = 255;

            if (
              currentColorHex &&
              currentColorHex == rgbToHex(red, green, blue)
            ) {
              markerCurentColorPos = [x, y];
            }

            data[index] = red;
            data[index + 1] = green;
            data[index + 2] = blue;
            data[index + 3] = alpha;
          }
        }

        ctx.putImageData(image, 0, 0);

        if (markerCurentColorPos) {
          drawPickerHandle(
            ctx,
            markerCurentColorPos[0] + radius,
            markerCurentColorPos[1] + radius
          );
        }
      }

      function xy2polar(x, y) {
        let r = Math.sqrt(x * x + y * y);
        let phi = Math.atan2(y, x);
        return [r, phi];
      }

      function rad2deg(rad) {
        return ((rad + Math.PI) / (2 * Math.PI)) * 360;
      }

      function rgbToHex(r, g, b) {
        return (
          "#" + ((1 << 24) | (r << 16) | (g << 8) | b).toString(16).slice(1)
        );
      }

      function hsv2rgb(hue, saturation, value) {
        let chroma = value * saturation;
        let hue1 = hue / 60;
        let x = chroma * (1 - Math.abs((hue1 % 2) - 1));
        let r1, g1, b1;
        if (hue1 >= 0 && hue1 <= 1) {
          [r1, g1, b1] = [chroma, x, 0];
        } else if (hue1 >= 1 && hue1 <= 2) {
          [r1, g1, b1] = [x, chroma, 0];
        } else if (hue1 >= 2 && hue1 <= 3) {
          [r1, g1, b1] = [0, chroma, x];
        } else if (hue1 >= 3 && hue1 <= 4) {
          [r1, g1, b1] = [0, x, chroma];
        } else if (hue1 >= 4 && hue1 <= 5) {
          [r1, g1, b1] = [x, 0, chroma];
        } else if (hue1 >= 5 && hue1 <= 6) {
          [r1, g1, b1] = [chroma, 0, x];
        }

        let m = value - chroma;
        let [r, g, b] = [r1 + m, g1 + m, b1 + m];

        // Change r,g,b values from [0,1] to [0,255]
        return [255 * r, 255 * g, 255 * b];
      }
    </script>
  </body>
</html>
    )";
