// html_page.h
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>Audio Recorder</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { 
      font-family: Arial; 
      text-align: center; 
      margin: 0;
      padding: 20px;
    }
    .button {
      padding: 15px 32px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 16px;
      margin: 4px 2px;
      cursor: pointer;
      border-radius: 4px;
      border: none;
      color: white;
    }
    .record {
      background-color: #ff4444;
    }
    .download {
      background-color: #4CAF50;
    }
    .status {
      margin: 20px 0;
      padding: 20px;
      background-color: #f0f0f0;
      border-radius: 4px;
    }
    .hidden {
      display: none;
    }
  </style>
</head>
<body>
  <h1>ESP32 Audio Recorder</h1>
  <div class="status">
    <p>Status: <span id="recordingStatus">Not recording</span></p>
    <p>Latest recording: <span id="filename">None</span></p>
  </div>
  
  <button id="recordButton" class="button record" onclick="toggleRecording()">Start Recording</button>
  <a id="downloadButton" href="/recording.wav" class="button download hidden" download>Download Recording</a>
  
  <script>
    let isRecording = false;
    const recordButton = document.getElementById('recordButton');
    const downloadButton = document.getElementById('downloadButton');
    
    function toggleRecording() {
      if (!isRecording) {
        startRecording();
      } else {
        stopRecording();
      }
    }
    
    function startRecording() {
      fetch('/start-recording')
        .then(response => response.json())
        .then(data => {
          if (data.success) {
            isRecording = true;
            recordButton.textContent = 'Stop Recording';
            downloadButton.classList.add('hidden');
            updateStatus();
          }
        });
    }
    
    function stopRecording() {
      fetch('/stop-recording')
        .then(response => response.json())
        .then(data => {
          if (data.success) {
            isRecording = false;
            recordButton.textContent = 'Start Recording';
            downloadButton.classList.remove('hidden');
            updateStatus();
          }
        });
    }
    
    function updateStatus() {
      fetch('/status')
        .then(response => response.json())
        .then(data => {
          document.getElementById('recordingStatus').textContent = 
            data.isRecording ? 'Recording...' : 'Not recording';
          document.getElementById('filename').textContent = 
            data.filename || 'None';
          
          // Sync button state with server status
          if (data.isRecording !== isRecording) {
            isRecording = data.isRecording;
            recordButton.textContent = isRecording ? 'Stop Recording' : 'Start Recording';
            downloadButton.classList.toggle('hidden', isRecording);
          }
        });
    }
    
    // Update status every second
    setInterval(updateStatus, 1000);
    updateStatus();
  </script>
</body>
</html>
)rawliteral";