import React, { useState } from 'react';
import './App.css';
import Header from './components/Header/Header';
import Sidebar from './components/Sidebar/Sidebar';
import MainContent from './components/Sidebar/MainContent/MainContent';
import DevMode from './components/DevMode/DevMode';
import LoadingScreen from './components/LoadingScreen/LoadingScreen';
import DirectoryViewer from './components/DevMode/DirectoryViewer/DirectoryViewer';
import UploadGenData from './components/UploadGenData/UploadGenData';

const App = () => {
  const [isDevMode, setIsDevMode] = useState(false);
  const [loading, setLoading] = useState(true);
  const [showDirectoryViewer, setShowDirectoryViewer] = useState(false);
  const [showUploadGenData, setShowUploadGenData] = useState(false);

  const handleToggleMode = (mode) => {
    setIsDevMode(mode);
    console.log(`Switched to ${mode ? 'Dev' : 'User'} Mode`);
  };

  const handleLoaded = () => {
    setLoading(false);
  };

  const handleOpenDirectoryViewer = () => {
    setShowDirectoryViewer(!showDirectoryViewer);
  };

  const handleOpenUploadGenData = () => {
    setShowUploadGenData(!showUploadGenData);
  };

  if (loading) {
    return <LoadingScreen onLoaded={handleLoaded} />;
  }

  return (
    <div className="app">
      <Header
        onToggleMode={handleToggleMode}
        onOpenDirectoryViewer={handleOpenDirectoryViewer}
        onOpenUploadGenData={handleOpenUploadGenData}
        isDevMode={isDevMode}
        showDirectoryViewer={showDirectoryViewer}
      />
      <div className="content">
        {showDirectoryViewer ? (
          <DirectoryViewer />
        ) : showUploadGenData ? (
          <UploadGenData />
        ) : isDevMode ? (
          <DevMode />
        ) : (
          <>
            <Sidebar />
            <MainContent />
          </>
        )}
      </div>
    </div>
  );
};

export default App;
