import React, { useState } from 'react';
import './DevMode.css';
import PredictComponent from './PredictComponent/PredictComponent';
import PandasComponent from './PandasComponent/PandasComponent';
import Algorithms from './Algorithms/Algorithms';
import GenAlgorithms from './GenAlgorithms/GenAlgorithms';
import RunGenUser from './RunGenUser/RunGenUser';
import DirectoryViewer from './DirectoryViewer/DirectoryViewer';
import ShareTasks from './ShareTasks/ShareTasks';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faChartLine, faDatabase, faCogs, faProjectDiagram, faTasks, faFolderOpen } from '@fortawesome/free-solid-svg-icons';

const DevMode = () => {
  const [activeComponent, setActiveComponent] = useState(null);

  const handleComponentSwitch = (component) => {
    if (activeComponent === component) {
      setActiveComponent(null);
    } else {
      setActiveComponent(component);
    }
  };

  return (
    <div className="dev-mode">
      <h2>Developer Mode</h2>
      <div className="button-group">
        <button
          onClick={() => handleComponentSwitch('predict')}
          className={activeComponent === 'predict' ? 'active' : ''}
        >
          <FontAwesomeIcon icon={faChartLine} /> {activeComponent === 'predict' ? 'Close Predict' : 'Open Predict'}
        </button>
        <button
          onClick={() => handleComponentSwitch('pandas')}
          className={activeComponent === 'pandas' ? 'active' : ''}
        >
          <FontAwesomeIcon icon={faDatabase} /> {activeComponent === 'pandas' ? 'Close Pandas' : 'Open Pandas'}
        </button>
        <button
          onClick={() => handleComponentSwitch('algorithms')}
          className={activeComponent === 'algorithms' ? 'active' : ''}
        >
          <FontAwesomeIcon icon={faCogs} /> {activeComponent === 'algorithms' ? 'Close Algorithms' : 'Open Algorithms'}
        </button>
        <button
          onClick={() => handleComponentSwitch('genAlgorithms')}
          className={activeComponent === 'genAlgorithms' ? 'active' : ''}
        >
          <FontAwesomeIcon icon={faProjectDiagram} /> {activeComponent === 'genAlgorithms' ? 'Close Gen Algorithms' : 'Open Gen Algorithms'}
        </button>
        <button
          onClick={() => handleComponentSwitch('runGenUser')}
          className={activeComponent === 'runGenUser' ? 'active' : ''}
        >
          <FontAwesomeIcon icon={faProjectDiagram} /> {activeComponent === 'runGenUser' ? 'Close User Partial Gen Algorithms' : 'Open User Partial Gen Algorithms'}
        </button>
        <button
          onClick={() => handleComponentSwitch('shareTasks')}
          className={activeComponent === 'shareTasks' ? 'active' : ''}
        >
          <FontAwesomeIcon icon={faTasks} /> {activeComponent === 'shareTasks' ? 'Close Share Tasks' : 'Open Share Tasks'}
        </button>
      </div>
      <div className={`component-container ${activeComponent ? 'open' : 'closed'}`}>
        {activeComponent === 'predict' && <PredictComponent />}
        {activeComponent === 'pandas' && <PandasComponent />}
        {activeComponent === 'algorithms' && <Algorithms />}
        {activeComponent === 'genAlgorithms' && <GenAlgorithms />}
        {activeComponent === 'runGenUser' && <RunGenUser />}
        {activeComponent === 'shareTasks' && <ShareTasks />}
      </div>
    </div>
  );
};

export default DevMode;
