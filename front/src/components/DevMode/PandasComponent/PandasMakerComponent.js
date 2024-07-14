import React, { useState, useEffect } from 'react';
import './PandasComponent.css';

const PandasMakerComponent = () => {
  const [mainDirectory1, setMainDirectory1] = useState('');
  const [selectedFolder, setSelectedFolder] = useState('');
  const [txtFiles, setTxtFiles] = useState([]);
  const [data, setData] = useState('');
  const [loading, setLoading] = useState(false);
  const [message, setMessage] = useState('');

  useEffect(() => {
    if (selectedFolder) {
      fetchTxtFiles(selectedFolder);
    }
  }, [selectedFolder]);

  const fetchTxtFiles = (folder) => {
    fetch('http://localhost:5001/api/get-txt-files', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ directory: folder }),
    })
      .then(response => response.json())
      .then(data => {
        setTxtFiles(data.files);
        setData(data.files.length > 0 ? `${folder}/${data.files[0]}` : '');
      })
      .catch(error => {
        console.error('Error fetching txt files:', error);
      });
  };

  const handleFolderChange = (event) => {
    const folder = event.target.value;
    setSelectedFolder(folder);
    setData(''); // Reset data selection
  };

  const handleFileChange = (event) => {
    const file = event.target.value;
    setData(`${selectedFolder}/${file}`);
  };

  const handlePandasMakerSubmit = () => {
    setLoading(true);
    setMessage('');
    const payload = {
      mainDirectory1,
      data,
    };

    fetch('http://localhost:5001/api/pandas-maker', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(payload),
    })
      .then(response => response.json())
      .then(data => {
        setLoading(false);
        setMessage('PandasMaker executed successfully');
        console.log('Success:', data);
      })
      .catch(error => {
        setLoading(false);
        setMessage('Error executing PandasMaker');
        console.error('Error:', error);
      });
  };

  return (
    <div className="pandas-component">
      <h2>Pandas Maker Component</h2>
      <div className="input-group">
        <label>Main Directory 1:</label>
        <input 
          type="text" 
          value={mainDirectory1} 
          onChange={(e) => setMainDirectory1(e.target.value)} 
          className="input-field"
        />
      </div>
      <div className="input-group">
        <label>Data Folder:</label>
        <select 
          value={selectedFolder} 
          onChange={handleFolderChange} 
          className="input-field"
        >
          <option value="">Select a folder</option>
          <option value="GenData">GenData</option>
          <option value="Data">Data</option>
          <option value="get">get</option>
        </select>
      </div>
      {selectedFolder && (
        <div className="input-group">
          <label>Data File:</label>
          <select 
            value={data.split('/').pop()} 
            onChange={handleFileChange} 
            className="input-field"
          >
            {txtFiles.map((file, index) => (
              <option key={index} value={file}>{file}</option>
            ))}
          </select>
        </div>
      )}
      <button 
        onClick={handlePandasMakerSubmit} 
        disabled={loading} 
        className={`submit-button ${loading ? 'loading' : ''}`}
      >
        {loading ? 'Running...' : 'Run PandasMaker'}
      </button>
      {message && <p className="message">{message}</p>}
    </div>
  );
};

export default PandasMakerComponent;
