const express = require('express');
const fs = require('fs');
const path = require('path');
const app = express();
const PORT = 5000;

// Разрешаем запросы с фронтенда
app.use((req, res, next) => {
  res.header('Access-Control-Allow-Origin', '*');
  res.header('Access-Control-Allow-Headers', 'Origin, X-Requested-With, Content-Type, Accept');
  next();
});

app.get('/api/files', (req, res) => {
  const directoryPath = req.query.path ? path.join('C:\\vscode\\pluto', req.query.path) : 'C:\\vscode\\pluto';

  fs.readdir(directoryPath, { withFileTypes: true }, (err, files) => {
    if (err) {
      console.error('Unable to scan directory:', err);
      return res.status(500).json({ error: 'Unable to scan directory', details: err.message });
    }

    const filesAndFolders = files.map(file => ({
      name: file.name,
      isDirectory: file.isDirectory()
    }));

    res.json(filesAndFolders);
  });
});

app.get('/api/file-content', (req, res) => {
  const filePath = path.join('C:\\vscode\\pluto', req.query.path);
  const extension = path.extname(filePath).toLowerCase();

  fs.readFile(filePath, extension === '.txt' || extension === '.csv' ? 'utf8' : null, (err, data) => {
    if (err) {
      console.error('Unable to read file:', err);
      return res.status(500).json({ error: 'Unable to read file', details: err.message });
    }

    if (extension === '.txt' || extension === '.csv') {
      res.json({ content: data });
    } else {
      res.json({ content: data.toString('base64') });
    }
  });
});

app.listen(PORT, () => {
  console.log(`Server is running on port ${PORT}`);
});
