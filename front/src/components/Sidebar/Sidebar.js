import React, { useState, useEffect } from 'react';
import './Sidebar.css';
import { CSSTransition } from 'react-transition-group';
import MainContent from './MainContent/MainContent';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faFolder, faFileAlt, faChevronLeft, faChevronRight, faChevronUp } from '@fortawesome/free-solid-svg-icons';

const Sidebar = () => {
  const [isOpen, setIsOpen] = useState(true);
  const [files, setFiles] = useState([]);
  const [paths, setPaths] = useState(['']);
  const [fileContent, setFileContent] = useState(null);
  const [fileType, setFileType] = useState('');

  useEffect(() => {
    const path = paths.join('/');
    fetch(`http://localhost:5001/api/files?path=${path}`)
      .then(response => {
        if (!response.ok) {
          throw new Error('Network response was not ok ' + response.statusText);
        }
        return response.json();
      })
      .then(data => setFiles(data))
      .catch(error => console.error('Error fetching files:', error));
  }, [paths]);

  const toggleSidebar = () => {
    setIsOpen(!isOpen);
  };

  const handleItemClick = (file) => {
    if (file.isDirectory) {
      setPaths([...paths, file.name]);
    } else {
      const filePath = [...paths, file.name].join('/');
      fetch(`http://localhost:5001/api/file-content?path=${filePath}`)
        .then(response => {
          if (!response.ok) {
            throw new Error('Network response was not ok ' + response.statusText);
          }
          return response.json();
        })
        .then(data => {
          const extension = file.name.split('.').pop().toLowerCase();
          if (['jpg', 'jpeg', 'png', 'gif'].includes(extension)) {
            setFileType('image');
            setFileContent(data.content); // Base64 content
          } else if (['txt'].includes(extension)) {
            setFileType('text');
            setFileContent(data.content);
          } else if (['csv'].includes(extension)) {
            setFileType('table');
            setFileContent(data.content);
          } else {
            setFileType('unsupported');
            setFileContent(null);
          }
        })
        .catch(error => console.error('Error fetching file content:', error));
    }
  };

  const handleBackClick = () => {
    setPaths(paths.slice(0, -1));
  };

  const fileExtensionsToHide = ['exe', 'pdb', 'obj', 'py', 'cpp', 'ilk', 'zip', 'json', 'pkl'];
  const directoriesToHide = ['front', '__pycache__', '.vscode', 'venv', 'venv1', '.git', 'send'];

  const filteredFiles = files.filter(file => {
    const extension = file.name.split('.').pop().toLowerCase();
    if (file.isDirectory) {
      return !directoriesToHide.includes(file.name);
    } else {
      return !fileExtensionsToHide.includes(extension);
    }
  });

  return (
    <div className="app-container">
      <div className={`sidebar-container ${isOpen ? 'open' : 'closed'}`}>
        <CSSTransition
          in={isOpen}
          timeout={200}
          classNames="sidebar"
          unmountOnExit
        >
          <div className="sidebar">
            <div className="sidebar-header">
              <button onClick={handleBackClick} disabled={paths.length === 1} className="back-button">
                <FontAwesomeIcon icon={faChevronUp} /> Back
              </button>
               <button className={`toggle-button ${isOpen ? 'toggle-button-open' : 'toggle-button-closed'}`} onClick={toggleSidebar}>
          <FontAwesomeIcon icon={isOpen ? faChevronLeft : faChevronRight} />
        </button>
            </div>
            <ul>
              {filteredFiles.map((file, index) => (
                <li key={index} className={file.isDirectory ? 'directory' : 'file'} onClick={() => handleItemClick(file)}>
                  <FontAwesomeIcon icon={file.isDirectory ? faFolder : faFileAlt} className="file-icon" />
                  <span className="file-name">{file.name}</span>
                </li>
              ))}
            </ul>
          </div>
        </CSSTransition>
        <button className={`op-button ${isOpen ? 'op-button-open' : 'op-button-closed'}`} onClick={toggleSidebar}>
          {console.log(isOpen)}
          <FontAwesomeIcon icon={isOpen ? faChevronLeft : faChevronRight} />
        </button>
      </div>
      <MainContent fileContent={fileContent} fileType={fileType} />
    </div>
  );
};

export default Sidebar;
