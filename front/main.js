const { app, BrowserWindow } = require('electron');
const path = require('path');
const { exec } = require('child_process');
const waitOn = require('wait-on');
const { spawn } = require('child_process');

let mainWindow;

async function createWindow() {
  mainWindow = new BrowserWindow({
    width: 1024,
    height: 768,
    webPreferences: {
      nodeIntegration: true,
      contextIsolation: false,
      enableRemoteModule: true,
    },
  });

  const isDev = await import('electron-is-dev').then(mod => mod.default);

  const startUrl = isDev
    ? 'http://localhost:3000'
    : `file://${path.join(__dirname, 'build/index.html')}`;

  if (isDev) {
    waitOn({ resources: [startUrl] }, (err) => {
      if (err) {
        console.error('Error waiting for the React server:', err);
        return;
      }
      mainWindow.loadURL(startUrl);
      mainWindow.webContents.openDevTools();
    });
  } else {
    mainWindow.loadURL(startUrl);
  }

  mainWindow.on('closed', () => (mainWindow = null));
}

app.on('ready', () => {
  createWindow();

  // Start the Flask server
  const flaskServerPath = path.join(__dirname, '..', 'main.py'); // Update this path accordingly
  const flaskServerDir = path.dirname(flaskServerPath);
  const pythonCommand = path.join(__dirname, '..', 'venv', 'Scripts', 'python.exe'); // Ensure this path is correct

  console.log(`Starting Flask server with command: ${pythonCommand} ${flaskServerPath}`);
  console.log(`Working directory: ${flaskServerDir}`);

  const flaskProcess = spawn(pythonCommand, [flaskServerPath], { cwd: flaskServerDir });

  flaskProcess.stdout.on('data', (data) => {
    console.log(`Flask stdout: ${data}`);
  });

  flaskProcess.stderr.on('data', (data) => {
    console.error(`Flask stderr: ${data}`);
  });

  flaskProcess.on('close', (code) => {
    console.log(`Flask process exited with code ${code}`);
  });
});

app.on('activate', () => {
  if (mainWindow === null) {
    createWindow();
  }
});
