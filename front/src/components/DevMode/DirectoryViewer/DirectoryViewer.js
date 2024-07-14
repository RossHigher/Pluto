import React, { useEffect, useState } from 'react';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faFolder, faFileAlt, faDownload } from '@fortawesome/free-solid-svg-icons';
import './DirectoryViewer.css';

const DirectoryViewer = () => {
  const [directories, setDirectories] = useState({});
  const [loading, setLoading] = useState(true);
  const [downloading, setDownloading] = useState(false);

  const fetchDirectories = async () => {
    try {
      const response = await fetch('http://localhost:5001/api/files-partial');
      const data = await response.json();
      setDirectories(data);
    } catch (error) {
      console.error('Error fetching directories:', error);
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    fetchDirectories();
  }, []);

  const handleDownload = async (filePath) => {
    setDownloading(true);
    const url = `http://localhost:5001/api/download-file?path=${encodeURIComponent(filePath)}`;
    const response = await fetch(url);
    if (response.ok) {
      const blob = await response.blob();
      const link = document.createElement('a');
      link.href = URL.createObjectURL(blob);
      link.download = filePath.split('/').pop();
      document.body.appendChild(link);
      link.click();
      link.remove();
    } else {
      console.error('Error downloading file');
    }
    setDownloading(false);
    fetchDirectories();
  };

  if (loading) {
    return <div className="loading">Loading...</div>;
  }

  return (
    <div className="directory-viewer">
      {directories.length === 0 && <div>No directories found</div>}
      {Object.keys(directories).map((directory, index) => (
        <div key={index} className="directory-container">
          <div className="directory-v">
            <FontAwesomeIcon icon={faFolder} className="icon" />
            <span>{directory}</span>
          </div>
          <div className="files-grid">
            {directories[directory].map((file, fileIndex) => (
              <div key={fileIndex} className="file-item">
                <FontAwesomeIcon icon={faFileAlt} className="icon" />
                <span>{file}</span>
                <button
                  className="download-button"
                  onClick={() => handleDownload(`${directory}${file}`)}
                  disabled={downloading}
                >
                  <FontAwesomeIcon icon={faDownload} />
                </button>
              </div>
            ))}
          </div>
        </div>
      ))}
    </div>
  );
};

export default DirectoryViewer;
