import React, { useState, useEffect } from 'react';
import './UploadGenData.css';

const UploadGenData = () => {
  const [files, setFiles] = useState([]);
  const [selectedFile, setSelectedFile] = useState('');
  const [message, setMessage] = useState('');

  useEffect(() => {
    const fetchFiles = async () => {
      try {
        const response = await fetch('http://localhost:5001/api/get-filenames?dirname=GenData');
        const data = await response.json();
        setFiles(data.filenames);
      } catch (error) {
        console.error('Error fetching files:', error);
      }
    };

    fetchFiles();
  }, []);

  const handleUpload = async () => {
    try {
      const response = await fetch('http://localhost:5001/api/upload-gen-data', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ filename: selectedFile }),
      });

      const data = await response.json();
      if (response.ok) {
        setMessage(`File ${selectedFile} uploaded successfully!`);
      } else {
        setMessage(`Failed to upload file: ${data.message}`);
      }
    } catch (error) {
      console.error('Error uploading file:', error);
      setMessage('Error uploading file.');
    }
  };

  return (
    <div className="upload-gen-data">
      <h3>Upload Gen Data</h3>
      <div className="input-group">
        <label htmlFor="fileSelect">Select a file:</label>
        <select
          id="fileSelect"
          value={selectedFile}
          onChange={(e) => setSelectedFile(e.target.value)}
        >
          <option value="" disabled>Select a file</option>
          {files.map((file, index) => (
            <option key={index} value={file}>{file}</option>
          ))}
        </select>
      </div>
      <button onClick={handleUpload} disabled={!selectedFile}>Upload to Yandex Cloud</button>
      {message && <p className="message">{message}</p>}
    </div>
  );
};

export default UploadGenData;
