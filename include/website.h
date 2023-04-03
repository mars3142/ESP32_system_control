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

      .field .control {
        width: 50%;
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
                      value="#000000"
                    />
                  </div>
                </div>

                <div class="field">
                  <label class="label">Helligkeit</label>
                  <div class="control">
                    <input
                      class="input slider"
                      name="brightnessDay"
                      id="brightnessDay"
                      type="range"
                      value="0"
                      min="0"
                      max="255"
                    />
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
                      id="colorNight"
                      value="#000000"
                    />
                  </div>
                </div>

                <div class="field">
                  <label class="label">Helligkeit</label>
                  <div class="control">
                    <input
                      class="input slider"
                      type="range"
                      name="brightnessNight"
                      id="brightnessNight"
                      value="0"
                      min="0"
                      max="255"
                    />
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
          const response =
            event.data ||
            `{"state":"day", "colorDay":"#000000", "brightnessDay":255, "colorNight":"#000000", "brightnessNight":0}`;
          try {
            const data = JSON.parse(response);
            setVal("colorDay", data.colorDay);
            setVal("brightnessDay", data.brightnessDay);
            setVal("colorNight", data.colorNight);
            setVal("brightnessNight", data.brightnessNight);
            setDN(data.state);
          } catch (e) {
            console.error(e);
          }
        });
      }

      function getVal(id) {
        return find(id).value;
      }

      function setVal(id, value) {
        return (find(id).value = value);
      }

      function find(id) {
        return document.getElementById(id);
      }

      function setDN(state) {
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
        const data = `colorDay=${getVal("colorDay")}&brightnessDay=${getVal(
          "brightnessDay"
        )}&colorNight=${getVal("colorNight")}&brightnessNight=${getVal(
          "brightnessNight"
        )}`;
        ws.send(data);
      }

      function sendDNState(state) {
        ws.send(state);
      }

      ws_connect();
    </script>
  </body>
</html>
    )";
