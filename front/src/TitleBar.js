import React from 'react';
import './TitleBar.css';
import { remote } from 'electron';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faWindowMinimize, faWindowMaximize, faTimes } from '@fortawesome/free-solid-svg-icons';

const TitleBar = () => {
  const handleMinimize = () => {
    remote.getCurrentWindow().minimize();
  };

  const handleMaximize = () => {
    const window = remote.getCurrentWindow();
    if (window.isMaximized()) {
      window.unmaximize();
    } else {
      window.maximize();
    }
  };

  const handleClose = () => {
    remote.getCurrentWindow().close();
  };

  return (
    <div className="title-bar">
      <div className="title-bar-controls">
        <button onClick={handleMinimize}><FontAwesomeIcon icon={faWindowMinimize} /></button>
        <button onClick={handleMaximize}><FontAwesomeIcon icon={faWindowMaximize} /></button>
        <button onClick={handleClose}><FontAwesomeIcon icon={faTimes} /></button>
      </div>
    </div>
  );
};

export default TitleBar;
