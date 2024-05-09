// Sources 
// Kaleidoscope code credit to Coding Train
// https://www.youtube.com/watch?v=R3C2giDfmO8
let serialOptions = { baudRate: 115200  };
let serial;

let symmetry = 6;
let angle = 360 / symmetry;
let saveButton;
let clearButton;
let slider;
let offset = 0;
let widthVal;
let widthFrac;
let isRainbow;
let hu;

let brushType = 0;     
let brushX = 0;         // Current brush x location 
let brushY = 0;         // Current brush y location 
let brushColor;         // Current brush color
 
let lastBrushX = 0;     // Last brush y position
let lastBrushY = 0;     // Last brush y position


function setup() {
  
  createCanvas(800, 800);
  angleMode(DEGREES);
  background(127);
  saveButton = createButton('save');
  saveButton.mousePressed(saveSnowflake);
  slider = createSlider(1, 32, 4, 0.1);
  isRainbow = false;
  colorMode(HSB, 255, 255, 255);

  // Setup Web Serial using serial.js
  serial = new Serial();
  serial.on(SerialEvents.CONNECTION_OPENED, onSerialConnectionOpened);
  serial.on(SerialEvents.CONNECTION_CLOSED, onSerialConnectionClosed);
  serial.on(SerialEvents.DATA_RECEIVED, onSerialDataReceived);
  serial.on(SerialEvents.ERROR_OCCURRED, onSerialErrorOccurred);
}

function saveSnowflake() {
  save('KaleidoArt.png');
}

function draw() {
  
  
  if (serial.isOpen()) {
    // Draw current brush stroke at current brush x,y position from serial
    drawBrushStroke(brushX, brushY);
  }
}

function drawBrushStroke() {
  translate(width / 2, height / 2);

  if (brushX > 0 && brushX < width && brushY > 0 && brushY < height) {
    let mx = brushX - width / 2;
    let my = brushY - height / 2;
    let pmx = lastBrushX - width / 2;
    let pmy = lastBrushY - height / 2;
      
    
      if (isRainbow) { 
        // Button pressed, use rainbow colors
        hu = noise(offset) * 255; 
        offset += 0.01;
        stroke(hu, 255, 255, 100);
      } else {
        // Normal case (black, gray, white) colors
        hu = map(sin(offset), -1,1,0,255);
        offset += 1;
        stroke(hu, 100);
      }
      let angle = 360 / symmetry;
      for (let i = 0; i < symmetry; i++) {
        rotate(angle);
        // Slider width based on pot input
        let sw = widthFrac;
        console.log("Slider val " + sw);
        strokeWeight(sw);
        line(mx, my, pmx, pmy);
        push();
        scale(1, -1);
        line(mx, my, pmx, pmy);
        pop();
      }
  }
}

 function onSerialErrorOccurred(eventSender, error) {
  console.log("onSerialErrorOccurred", error);
}

function onSerialConnectionOpened(eventSender) {
  console.log("onSerialConnectionOpened");
}

function onSerialConnectionClosed(eventSender) {
  console.log("onSerialConnectionClosed");
}

function onSerialDataReceived(eventSender, newData) {
  console.log("onSerialDataReceived", newData);

  if(!newData.startsWith("#")){
    // Clear screen
    if(newData.toLowerCase().startsWith("clear")) {
      background(127);
    }
    
    if (newData.toLowerCase().startsWith("color")) {
      isRainbow = !isRainbow;
    }
    
    let startIndex = 0;
    let endIndex = newData.indexOf(",");
    if(endIndex != -1){
      // Parse x location (normalized between 0 and 1)
      let strBrushXFraction = newData.substring(startIndex, endIndex).trim();
      let xFraction = parseFloat(strBrushXFraction);

      // Parse y location (normalized between 0 and 1)
      startIndex = endIndex + 1;
      endIndex = newData.length;
      let strBrushYFraction = newData.substring(startIndex, endIndex).trim();
      let yFraction = parseFloat(strBrushYFraction); 
      
      // Parse brush width information
      widthVal = newData.substring(startIndex, endIndex).trim();
      const widthArray = widthVal.split(",");
      widthFrac = parseFloat(widthArray[1]) / 32.0;

      // Set relevant global variables for brush x,y location in pixels
      lastBrushX = brushX;
      lastBrushY = brushY;

      brushX = xFraction * width;
      brushY = yFraction * height;
    }
  }
}


function mouseClicked() {
  if (!serial.isOpen()) {
    serial.connectAndOpen(null, serialOptions);
  }
}