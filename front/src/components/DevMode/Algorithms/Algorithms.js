import React, { useState } from 'react';
import './Algorithms.css';

const Algorithms = () => {
  const [ns, setNs] = useState(['', '']);
  const [ms, setMs] = useState(['']);
  const [interval, setInterval] = useState(['', '']);
  const [maxMatrixGenerated, setMaxMatrixGenerated] = useState('');
  const [error, setError] = useState('');
  const [loading, setLoading] = useState(false);
  const [response, setResponse] = useState('');

  const handleNsChange = (index, value) => {
    const newNs = [...ns];
    newNs[index] = value;
    setNs(newNs);
  };

  const handleMsChange = (index, value) => {
    const newMs = [...ms];
    newMs[index] = value;
    setMs(newMs);
  };

  const addMsField = () => {
    setMs([...ms, '']);
  };

  const handleIntervalChange = (index, value) => {
    const newInterval = [...interval];
    newInterval[index] = value;
    setInterval(newInterval);
  };

  const handleSubmit = async (e) => {
    e.preventDefault();
    if (ns.includes('') || interval.includes('') || maxMatrixGenerated === '' || ms.includes('')) {
      setError('All fields are required');
      return;
    }
    setError('');
    setLoading(true);

    const payload = {
      Ns: ns.join(', '),
      Ms: ms.join(', '),
      Interval: interval.join(', '),
      maxMatrixGenerated: maxMatrixGenerated
    };

    try {
      const response = await fetch('http://localhost:5001/api/run-algorithms', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(payload),
      });
      const data = await response.json();
      setResponse(data.stdout + "\n" + data.stderr);
    } catch (error) {
      setError('Error submitting the form');
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="algorithms-component">
      <h3>Algorithms</h3>
      <form onSubmit={handleSubmit}>
        <div className="input-group">
          <label htmlFor="ns1">Ns:</label>
          <input
            id="ns1"
            type="number"
            value={ns[0]}
            placeholder="Enter first Ns value"
            onChange={(e) => handleNsChange(0, e.target.value)}
          />
          <input
            id="ns2"
            type="number"
            value={ns[1]}
            placeholder="Enter second Ns value"
            onChange={(e) => handleNsChange(1, e.target.value)}
          />
        </div>
        <div className="input-group">
          <label>Ms:</label>
          {ms.map((value, index) => (
            <input
              key={index}
              type="number"
              value={value}
              placeholder={`Enter Ms value ${index + 1}`}
              onChange={(e) => handleMsChange(index, e.target.value)}
            />
          ))}
          <button type="button" onClick={addMsField} className="add-button">Add Ms</button>
        </div>
        <div className="input-group">
          <label htmlFor="interval1">Interval:</label>
          <input
            id="interval1"
            type="number"
            value={interval[0]}
            placeholder="Enter first interval value"
            onChange={(e) => handleIntervalChange(0, e.target.value)}
          />
          <input
            id="interval2"
            type="number"
            value={interval[1]}
            placeholder="Enter second interval value"
            onChange={(e) => handleIntervalChange(1, e.target.value)}
          />
        </div>
        <div className="input-group">
          <label htmlFor="maxMatrixGenerated">Max Matrix Generated:</label>
          <input
            id="maxMatrixGenerated"
            type="number"
            value={maxMatrixGenerated}
            placeholder="Enter max matrix generated"
            onChange={(e) => setMaxMatrixGenerated(e.target.value)}
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
          </div>
        </div>
      )}
    </div>
  );
};

export default Algorithms;
