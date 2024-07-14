import React, { useState, useEffect } from 'react';
import './ShareTasks.css';

const ShareTasks = () => {
  const [filenames, setFilenames] = useState([]);
  const [selectedFile, setSelectedFile] = useState('');
  const [inviteNumber, setInviteNumber] = useState(1);
  const [message, setMessage] = useState('');
  const [loading, setLoading] = useState(false);

  useEffect(() => {
    const fetchFilenames = async () => {
      try {
        const response = await fetch('http://localhost:5001/api/get-filenames?dirname=T');
        const data = await response.json();
        setFilenames(data.filenames);
      } catch (error) {
        console.error('Error fetching filenames:', error);
      }
    };

    fetchFilenames();
  }, []);

  const handleShareTasks = async () => {
    setLoading(true);
    const payload = {
      tasksFileNameSimple: selectedFile,
      inviteComputerSystemNumber: inviteNumber,
    };

    try {
      const response = await fetch('http://localhost:5001/api/share-tasks', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(payload),
      });
      const data = await response.json();
      setMessage(data.message);
    } catch (error) {
      console.error('Error sharing tasks:', error);
      setMessage('Error sharing tasks');
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="share-tasks">
      <h3>Share Tasks</h3>
      <div className="input-group">
        <label htmlFor="fileSelect">Select File:</label>
        <select
          id="fileSelect"
          value={selectedFile}
          onChange={(e) => setSelectedFile(e.target.value)}
        >
          <option value="" disabled>Select a file</option>
          {filenames.map((file, index) => (
            <option key={index} value={file}>{file}</option>
          ))}
        </select>
      </div>
      <div className="input-group">
        <label htmlFor="inviteNumber">Invite Number:</label>
        <input
          id="inviteNumber"
          type="number"
          value={inviteNumber}
          onChange={(e) => setInviteNumber(e.target.value)}
          min="1"
        />
      </div>
      <button onClick={handleShareTasks} disabled={loading || !selectedFile}>
        {loading ? 'Processing...' : 'Share Tasks'}
      </button>
      {message && <p>{message}</p>}
    </div>
  );
};

export default ShareTasks;
