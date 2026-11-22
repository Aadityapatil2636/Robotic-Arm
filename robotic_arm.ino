
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

const char* ssid = "RobotArm";
const char* password = "12345678";

WebServer server(80);

Servo servo[4];
const int servoPins[4] = {13, 14, 25, 26};

int currentAngle[4] = {90, 90, 90, 90};
int targetAngle[4] = {90, 90, 90, 90};

const int MIN_ANGLE = 0;
const int MAX_ANGLE = 180;

const int STEP_SIZE[4] = {6, 6, 6, 12};
const int MOVE_STEP[4] = {1, 1, 1, 2};

const unsigned long MOVE_INTERVAL = 8;

unsigned long lastMove = 0;

const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no">
<title>ROBOT ARM</title>

<style>
* { box-sizing: border-box; -webkit-tap-highlight-color: transparent; }

body {
  margin: 0;
  padding: 18px;
  min-height: 100vh;
  background: radial-gradient(circle at top, #14233f 0%, #0b1120 60%);
  color: #fff;
  font-family: system-ui, -apple-system, "Segoe UI", Roboto, Arial, sans-serif;
  user-select: none;
  -webkit-user-select: none;
}

.container { max-width: 620px; margin: auto; }

header { text-align: center; padding: 14px 0 18px; }

header h1 {
  margin: 0;
  font-size: 28px;
  letter-spacing: 4px;
  color: #38bdf8;
  text-shadow: 0 0 18px rgba(56,189,248,.45);
}

header p {
  margin: 6px 0 0;
  color: #94a3b8;
  font-size: 12px;
  letter-spacing: 2px;
}

.status {
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 10px;
  background: #121f33;
  border: 1px solid #1e3a52;
  border-radius: 12px;
  padding: 12px;
  margin-bottom: 18px;
  font-size: 13px;
  letter-spacing: 1px;
  color: #4ade80;
}

.dot {
  width: 10px;
  height: 10px;
  border-radius: 50%;
  background: #4ade80;
  box-shadow: 0 0 10px #4ade80;
  animation: pulse 1.6s infinite;
}

.status.error { color: #f87171; }

.status.error .dot {
  background: #f87171;
  box-shadow: 0 0 10px #f87171;
  animation: none;
}

@keyframes pulse { 50% { opacity: .35; } }

.card {
  background: linear-gradient(145deg, #182640, #101a2c);
  border: 1px solid #263850;
  border-radius: 18px;
  padding: 16px 18px;
  margin-bottom: 14px;
  box-shadow: 0 6px 18px rgba(0,0,0,.3);
}

.row {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
}

.badge {
  width: 38px;
  height: 38px;
  flex: none;
  border-radius: 12px;
  background: #0f2a40;
  border: 1px solid #1f4a6b;
  color: #38bdf8;
  display: flex;
  align-items: center;
  justify-content: center;
  font-weight: bold;
  font-size: 15px;
}

.info { flex: 1; min-width: 0; }

.info h2 {
  margin: 0 0 3px;
  font-size: 16px;
  letter-spacing: 1px;
}

.info p {
  margin: 0;
  color: #94a3b8;
  font-size: 12px;
}

.angle { text-align: right; min-width: 56px; }

.angle b {
  font-size: 22px;
  color: #38bdf8;
  font-variant-numeric: tabular-nums;
}

.angle span { color: #64748b; font-size: 12px; }

.bar {
  height: 8px;
  border-radius: 8px;
  background: #0b1424;
  border: 1px solid #1e2f47;
  margin: 14px 0;
  overflow: hidden;
}

.fill {
  height: 100%;
  width: 50%;
  background: linear-gradient(90deg, #0284c7, #38bdf8);
  border-radius: 8px;
  transition: width .15s linear;
}

.controls { display: flex; gap: 12px; }

.arrow {
  flex: 1;
  height: 54px;
  border-radius: 15px;
  border: 1px solid #334e68;
  background: #243750;
  color: #38bdf8;
  display: flex;
  align-items: center;
  justify-content: center;
  cursor: pointer;
  touch-action: none;
  transition: .12s;
}

.arrow svg { width: 26px; height: 26px; }

.arrow:active, .arrow.hold {
  background: #0284c7;
  color: #fff;
  transform: scale(.96);
  box-shadow: 0 0 16px rgba(2,132,199,.6);
}

.home {
  width: 100%;
  padding: 16px;
  border: none;
  border-radius: 14px;
  margin-top: 6px;
  background: linear-gradient(90deg, #0284c7, #0ea5e9);
  color: #fff;
  font-size: 15px;
  font-weight: bold;
  letter-spacing: 2px;
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 10px;
  cursor: pointer;
}

.home svg { width: 22px; height: 22px; }

.home:active {
  filter: brightness(.85);
  transform: scale(.99);
}

footer {
  text-align: center;
  color: #64748b;
  font-size: 11px;
  letter-spacing: 2px;
  padding: 20px;
}

@media (max-width: 380px) {
  .card { padding: 12px; }
  .arrow { height: 48px; }
}
</style>
</head>

<body>
<div class="container">

<header>
  <h1>ROBOT ARM</h1>
  <p>4-AXIS MOTION CONTROL</p>
</header>

<div class="status" id="status">
  <span class="dot"></span>
  <span id="statusText">SYSTEM CONNECTED</span>
</div>

<div id="servoCards"></div>

<button class="home" onclick="homePosition()">
  <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
    <path d="M3 11l9-8 9 8"/>
    <path d="M5 10v10h5v-6h4v6h5V10"/>
  </svg>
  HOME POSITION
</button>

<footer>ESP32 WIRELESS SERVO CONTROLLER</footer>

</div>

<script>
const names = ["BASE", "SHOULDER", "ELBOW", "WRIST"];
const details = ["Base rotation", "Vertical arm movement", "Elbow movement", "Wrist movement"];

const MIN = 10, MAX = 170;

const LEFT = '<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="3" stroke-linecap="round" stroke-linejoin="round"><path d="M15 5l-7 7 7 7"/></svg>';

const RIGHT = '<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="3" stroke-linecap="round" stroke-linejoin="round"><path d="M9 5l7 7-7 7"/></svg>';

const cards = document.getElementById("servoCards");

let html = "";

for (let i = 0; i < 4; i++) {
  html += `
  <div class="card">
    <div class="row">
      <div class="badge">${i + 1}</div>
      <div class="info">
        <h2>${names[i]}</h2>
        <p>${details[i]}</p>
      </div>
      <div class="angle">
        <b id="a${i}">90</b><span>&deg;</span>
      </div>
    </div>
    <div class="bar">
      <div class="fill" id="f${i}"></div>
    </div>
    <div class="controls">
      <button class="arrow" data-id="${i}" data-dir="-1">${LEFT}</button>
      <button class="arrow" data-id="${i}" data-dir="1">${RIGHT}</button>
    </div>
  </div>`;
}

cards.innerHTML = html;

function setStatus(text, error) {
  document.getElementById("status").className = "status" + (error ? " error" : "");
  document.getElementById("statusText").textContent = text;
}

function send(url) {
  fetch(url)
    .then(r => {
      if (!r.ok) throw new Error();
      return r.text();
    })
    .then(t => setStatus(t, false))
    .catch(() => setStatus("CONNECTION ERROR", true));
}

function moveServo(id, dir) {
  send(`/move?id=${id}&dir=${dir}`);
}

function homePosition() {
  send("/home");
}

let timer = null;

function startHold(btn) {
  stopHold();

  btn.classList.add("hold");

  const id = btn.dataset.id;
  const dir = btn.dataset.dir;

  moveServo(id, dir);

  timer = setInterval(() => moveServo(id, dir), 100);
}

function stopHold() {
  if (timer) {
    clearInterval(timer);
    timer = null;
  }

  document.querySelectorAll(".arrow.hold").forEach(b => b.classList.remove("hold"));
}

document.querySelectorAll(".arrow").forEach(btn => {
  btn.addEventListener("pointerdown", e => {
    e.preventDefault();
    startHold(btn);
  });
});

["pointerup", "pointercancel", "pointerleave"].forEach(ev =>
  document.addEventListener(ev, stopHold)
);

document.addEventListener("contextmenu", e => e.preventDefault());

function updateAngles() {
  fetch("/state")
    .then(r => r.text())
    .then(t => {
      t.split(",").forEach((v, i) => {
        const a = parseInt(v);

        document.getElementById("a" + i).textContent = a;

        document.getElementById("f" + i).style.width =
          ((a - MIN) / (MAX - MIN) * 100) + "%";
      });
    })
    .catch(() => {});
}

setInterval(updateAngles, 300);
updateAngles();
</script>

</body>
</html>
)rawliteral";

void handleMove() {
  if (!server.hasArg("id") || !server.hasArg("dir")) {
    server.send(400, "text/plain", "INVALID REQUEST");
    return;
  }

  int id = server.arg("id").toInt();
  int direction = server.arg("dir").toInt();

  if (id < 0 || id >= 4 || (direction != 1 && direction != -1)) {
    server.send(400, "text/plain", "INVALID COMMAND");
    return;
  }

  targetAngle[id] += direction * STEP_SIZE[id];
  targetAngle[id] = constrain(targetAngle[id], MIN_ANGLE, MAX_ANGLE);

  String msg = "SERVO ";
  msg += String(id + 1);
  msg += " MOVING";

  server.send(200, "text/plain", msg);
}

void handleHome() {
  for (int i = 0; i < 4; i++) {
    targetAngle[i] = 90;
  }

  server.send(200, "text/plain", "RETURNING TO HOME");
}

void handleState() {
  String s = "";

  for (int i = 0; i < 4; i++) {
    s += String(currentAngle[i]);

    if (i < 3) s += ",";
  }

  server.send(200, "text/plain", s);
}

void smoothServoMovement() {
  unsigned long now = millis();

  if (now - lastMove < MOVE_INTERVAL) return;

  lastMove = now;

  for (int i = 0; i < 4; i++) {
    if (currentAngle[i] < targetAngle[i]) {
      currentAngle[i] = min(currentAngle[i] + MOVE_STEP[i], targetAngle[i]);
      servo[i].write(currentAngle[i]);
    }
    else if (currentAngle[i] > targetAngle[i]) {
      currentAngle[i] = max(currentAngle[i] - MOVE_STEP[i], targetAngle[i]);
      servo[i].write(currentAngle[i]);
    }
  }
}

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 4; i++) {
    servo[i].setPeriodHertz(50);
    servo[i].attach(servoPins[i], 600, 2300);
    servo[i].write(90);
  }

  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.println("ROBOT ARM STARTED");
  Serial.print("Connect to WiFi: ");
  Serial.println(ssid);
  Serial.print("Website IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html; charset=utf-8", webpage);
  });

  server.on("/move", HTTP_GET, handleMove);
  server.on("/home", HTTP_GET, handleHome);
  server.on("/state", HTTP_GET, handleState);

  server.begin();

  Serial.println("WEB SERVER READY");
}

void loop() {
  server.handleClient();
  smoothServoMovement();
}
