import React, { useState } from 'react';
import './PandasComponent.css';

const PandasMakerExcelComponent = () => {
  const [mainDirectory2, setMainDirectory2] = useState('');
  const [excelDirectoryRead, setExcelDirectoryRead] = useState('');
  const [loading, setLoading] = useState(false);
  const [message, setMessage] = useState('');

  const handlePandasMakerExcelSubmit = () => {
    setLoading(true);
    setMessage('');
    const payload = {
      mainDirectory2,
      excelDirectoryRead,
    };

    fetch('http://localhost:5001/api/pandas-maker-excel', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(payload),
    })
      .then(response => response.json())
      .then(data => {
        setLoading(false);
        setMessage('PandasMakerExcel executed successfully');
        console.log('Success:', data);
      })
      .catch(error => {
        setLoading(false);
        setMessage('Error executing PandasMakerExcel');
        console.error('Error:', error);
      });
  };

  return (
    <div className="pandas-component">
      <h2>Pandas Maker Excel Component</h2>
      <div className="input-group">
        <label>Main Directory 2:</label>
        <input type="text" value={mainDirectory2} onChange={(e) => setMainDirectory2(e.target.value)} />
      </div>
      <div className="input-group">
        <label>Excel Directory Read:</label>
        <input type="text" value={excelDirectoryRead} onChange={(e) => setExcelDirectoryRead(e.target.value)} />
      </div>
      <button onClick={handlePandasMakerExcelSubmit} disabled={loading}>
        {loading ? 'Running...' : 'Run PandasMakerExcel'}
      </button>
      {message && <p className="message">{message}</p>}
    </div>
  );
};

export default PandasMakerExcelComponent;
