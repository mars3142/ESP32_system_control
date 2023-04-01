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
        const apiUrl = "http://eisenbahn.local";
        let dnState = "";

        const webSocket = new WebSocket("ws://eisenbahn.local/state");

        webSocket.onopen = () => {
            console.log("Connected to websocket");
        };

        webSocket.onmessage = (event) => {
            console.log("Message from server: ", event.data);
        };

        function getVal(id) {
            return find(id).value;
        }

        function setVal(id, value) {
            console.log(`(document.getElementById(${id}).value = ${value})`);
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

        function mkRequest(method, url, data, callback) {
            console.log(method, url, data);
            find("submit").classList.add("is-loading");
            const r = new XMLHttpRequest();
            r.open(method, url, true);
            r.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
            r.onreadystatechange = function () {
            find("submit").classList.remove("is-loading");
            if (r.readyState != 4 || r.status != 200) {
                return;
            }
            find("submit").classList.remove("is-loading");
            callback(r);
            };
            r.send(data || "");
        }

        function loadData() {
            mkRequest("GET", `${apiUrl}/status`, "", (r) => {
            const response =
                r.responseText ||
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

        function sendData() {
            const data = `colorDay=${getVal("colorDay")}&brightnessDay=${getVal(
            "brightnessDay"
            )}&colorNight=${getVal("colorNight")}&brightnessNight=${getVal(
            "brightnessNight"
            )}`;

            mkRequest("POST", `${apiUrl}/config`, data, (r) => {
            loadData();
            });
        }

        function sendDNState(state) {
            const data = `state=${state}`;
            mkRequest("POST", `${apiUrl}/state`, data, (r) => {
            loadData();
            });
        }

        // init load
        loadData();
        </script>
    </body>
    </html>
    )";
