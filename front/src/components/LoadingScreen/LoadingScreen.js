import React, { useEffect } from 'react';
import './LoadingScreen.css';
import logo from './plutonium_fatlipmhl8ce.svg'; // Adjust the path to where your logo is stored

const LoadingScreen = ({ onLoaded }) => {
  useEffect(() => {
    const timer = setTimeout(() => {
      onLoaded();
    }, 2000); // Duration of the loading screen animation

    return () => clearTimeout(timer);
  }, [onLoaded]);

  return (
    <div className="loading-screen">
      <div className='logo-image-container'>
        <img src={logo} alt="logo" className="logo-image" />
      </div>
      <div className="logo-text">Pluto: Gen Algorithm</div>
      <div className="loading-bar-container">
        <div className="loading-bar"></div>
      </div>
    </div>
  );
};

export default LoadingScreen;
