import React, { useState } from 'react';
import './Header.css';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faToggleOn, faToggleOff, faFolderOpen, faUpload, faAtom } from '@fortawesome/free-solid-svg-icons';

const Header = ({ onToggleMode, onOpenDirectoryViewer, isDevMode, showDirectoryViewer, onOpenUploadGenData }) => {
  const [loading, setLoading] = useState(false);

  const toggleMode = () => {
    if (showDirectoryViewer) {
      onOpenDirectoryViewer(); // Close DirectoryViewer if open
    }
    onToggleMode(!isDevMode);
  };

  const toggleDirectoryViewer = () => {
    if (isDevMode) {
      onToggleMode(false); // Close DevMode if open
    }
    onOpenDirectoryViewer();
  };

  const toggleUploadGenData = () => {
    onOpenUploadGenData();
  };

  const handleLogoClick = async () => {
    setLoading(true);
    try {
      const response = await fetch('http://localhost:5001/api/gen-data-unite', {
        method: 'POST',
      });
      if (!response.ok) {
        throw new Error('Failed to generate data');
      }
      const result = await response.json();
      console.log('Data generated successfully:', result);
    } catch (error) {
      console.error('Error generating data:', error);
    } finally {
      setLoading(false);
    }
  };

  return (
    <header className="header">
      <div className="header-content">
        <div className="logo" onClick={handleLogoClick} style={{ cursor: 'pointer' }}>
          <FontAwesomeIcon icon={faAtom} className="logo-icon" />
          <h1 className="title">Pluto: Gen Algorithm</h1>
        </div>
        <div className="buttons">
          <button className="upload-gen-data-button" onClick={toggleUploadGenData}>
            <FontAwesomeIcon icon={faUpload} />
            {' Upload Gen Data'}
          </button>
          <button className="dev-mode-button" onClick={toggleMode}>
            <FontAwesomeIcon icon={isDevMode ? faToggleOn : faToggleOff} />
            {isDevMode ? ' User Mode' : ' Dev Mode'}
          </button>
          <button className="directory-viewer-button" onClick={toggleDirectoryViewer}>
            <FontAwesomeIcon icon={faFolderOpen} />
            {showDirectoryViewer ? ' Close Support' : ' Open Support'}
          </button>
        </div>
      </div>
      {loading && <div className="loading-indicator">Loading...</div>}
    </header>
  );
};

export default Header;
