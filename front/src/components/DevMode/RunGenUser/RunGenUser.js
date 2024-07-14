import React, { useState, useEffect } from 'react';
import './RunGenUser.css';

const GenAlgorithms = () => {
  const [readFile, setReadFile] = useState('');
  const [z, setZ] = useState('');
  const [error, setError] = useState('');
  const [loading, setLoading] = useState(false);
  const [response, setResponse] = useState('');
  const [filenames, setFilenames] = useState([]);
  const [estimatedTime, setEstimatedTime] = useState(0);
  const [countdown, setCountdown] = useState(0);

  useEffect(() => {
    fetchFilenames('DownloadedTasks'); // Call with the default directory 'T'
  }, []);

  useEffect(() => {
    let timer;
    if (loading && countdown > 0) {
      timer = setInterval(() => {
        setCountdown((prev) => (prev > 0 ? prev - 1 : 0));
      }, 1000);
    }
    return () => clearInterval(timer);
  }, [loading, countdown]);

  const fetchFilenames = async (dirname) => {
    try {
      const response = await fetch(`http://localhost:5001/api/get-filenames?dirname=${dirname}`);
      const data = await response.json();
      setFilenames(data.filenames);
    } catch (error) {
      console.error('Error fetching filenames:', error);
    }
  };

  const estimateRuntime = async () => {
    const payload = {
      readFile,
      Z: z
    };
    const response = await fetch('http://localhost:5001/api/estimate-runtime', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(payload),
    });
    const data = await response.json();
    console.log(`Estimated time (ms): ${data.estimated_time}`);  // Debug print
    setEstimatedTime(data.estimated_time);
    setCountdown(Math.ceil(data.estimated_time / 1000)); // Convert milliseconds to seconds
  };

  const handleSubmit = async (e) => {
    e.preventDefault();
    if (readFile === '' || z === '') {
      setError('All fields are required');
      return;
    }
    setError('');
    setLoading(true);

    await estimateRuntime();

    const payload = {
      readFile,
      Z: z
    };

    try {
      const response = await fetch('http://localhost:5001/api/run-genetic-algorithms-user', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(payload),
      });
      const data = await response.json();
      setResponse(data.message);
    } catch (error) {
      setError('Error submitting the form');
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="gen-algorithms-component">
      <h3>Gen Algorithms</h3>
      <form onSubmit={handleSubmit}>
        <div className="input-group">
          <label htmlFor="readFile">Read File:</label>
          <select
            id="readFile"
            value={readFile}
            onChange={(e) => setReadFile(e.target.value)}
          >
            <option value="" disabled>Select a file</option>
            {filenames.map((file, index) => (
              <option key={index} value={file}>{file}</option>
            ))}
          </select>
        </div>
        <div className="input-group">
          <label htmlFor="z">Z:</label>
          <input
            id="z"
            type="number"
            value={z}
            placeholder="Enter a number"
            onChange={(e) => setZ(e.target.value)}
          />
        </div>
        {error && <p className="error-message">{error}</p>}
        <button type="submit" className="submit-button" disabled={loading}>Submit</button>
        {response && <pre className="response-output">{response}</pre>}
      </form>

      {loading && (
        <div className="loading-modal">
          <div className="loading-content">
            <div className="spinner"></div>
            <p>Algorithm is running, please wait...</p>
            <p>Estimated time remaining: {countdown} seconds</p>
          </div>
        </div>
      )}
    </div>
  );
};

export default GenAlgorithms;
