import React, { useState } from 'react';
import './PredictComponent.css';

const PredictComponent = () => {
  const [mainDirectory, setMainDirectory] = useState('');
  const [algorithms, setAlgorithms] = useState([]);
  const [algorithm, setAlgorithm] = useState('');
  const [difference, setDifference] = useState('');
  const [predictCnst, setPredictCnst] = useState(null);
  const [someValue, setSomeValue] = useState('');
  const [predictResult, setPredictResult] = useState(null);
  const [error, setError] = useState('');

  const handleDirectoryChange = async (event) => {
    const directoryPath = event.target.value;
    setMainDirectory(directoryPath);

    // Fetch algorithms from the server
    try {
      const response = await fetch('http://localhost:5001/api/get-algorithms', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ mainDirectory: directoryPath }),
      });
      const data = await response.json();
      setAlgorithms(data.algorithms || []);
      setAlgorithm(data.algorithms[0] || '');
    } catch (error) {
      console.error('Error fetching algorithms:', error);
      setError('Error fetching algorithms');
    }
  };

  const handlePredictConstSubmit = () => {
    if (!mainDirectory || !algorithm || !difference) {
      setError('All fields are required');
      return;
    }

    const payload = {
      mainDirectory,
      Algorithm: algorithm,
      difference: parseFloat(difference),
    };

    fetch('http://localhost:5001/api/predict-const', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(payload),
    })
      .then(response => response.json())
      .then(data => {
        setPredictCnst(data.predictCnst);
        console.log('Predict Const Success:', data);
        setError('');
      })
      .catch(error => {
        console.error('Error in Predict Const:', error);
        setError('Error in Predict Const');
      });
  };

  const handlePredictValueSubmit = () => {
    if (!predictCnst || !someValue) {
      setError('All fields are required');
      return;
    }

    const payload = {
      predictCnst,
      someValue: parseInt(someValue, 10),
    };

    fetch('http://localhost:5001/api/predict-value', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(payload),
    })
      .then(response => response.json())
      .then(data => {
        setPredictResult(data.result);
        console.log('Predict Value Success:', data);
        setError('');
      })
      .catch(error => {
        console.error('Error in Predict Value:', error);
        setError('Error in Predict Value');
      });
  };

  return (
    <div className="predict-component">
      <h2>Predict Component</h2>
      <div className="input-group">
        <label>Main Directory:</label>
        <input
          type="text"
          value={mainDirectory}
          onChange={handleDirectoryChange}
          placeholder="Enter directory path"
        />
        <span className="selected-directory">{mainDirectory}</span>
      </div>
      <div className="input-group">
        <label>Algorithm:</label>
        <select value={algorithm} onChange={(e) => setAlgorithm(e.target.value)}>
          {algorithms.map((alg, index) => (
            <option key={index} value={alg}>{alg}</option>
          ))}
        </select>
      </div>
      <div className="input-group">
        <label>Difference:</label>
        <input
          type="number"
          step="0.01"
          value={difference}
          onChange={(e) => setDifference(e.target.value)}
        />
      </div>
      <button onClick={handlePredictConstSubmit}>Calculate PredictConst</button>
      {predictCnst !== null && (
        <div className="input-group">
          <label>Enter Value:</label>
          <input
            type="number"
            value={someValue}
            onChange={(e) => setSomeValue(e.target.value)}
          />
          <button onClick={handlePredictValueSubmit}>Submit Value</button>
        </div>
      )}
      {predictResult !== null && (
        <div className="result">
          <p>Predict Result: {predictResult}</p>
        </div>
      )}
      {error && <p className="error-message">{error}</p>}
    </div>
  );
};

export default PredictComponent;
