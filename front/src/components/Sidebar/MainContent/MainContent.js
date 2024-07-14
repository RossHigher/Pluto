import React from 'react';
import './MainContent.css';
import Papa from 'papaparse';

const MainContent = ({ fileContent, fileType }) => {
  if (!fileContent) {
    return <div className="main-content content-placeholder"></div>;
  }

  let content;
  switch (fileType) {
    case 'image':
      content = <img src={`data:image/jpeg;base64,${fileContent}`} alt="file content" />;
      break;
    case 'text':
      content = <pre>{fileContent}</pre>;
      break;
    case 'table':
      const parsedData = Papa.parse(fileContent, { header: false }).data;
      content = (
        <table>
          <thead>
            <tr>
              {parsedData[0].map((cell, index) => (
                <th key={index}>{cell}</th>
              ))}
            </tr>
          </thead>
          <tbody>
            {parsedData.slice(1).map((row, index) => (
              <tr key={index}>
                {row.map((cell, cellIndex) => (
                  <td key={cellIndex}>{cell}</td>
                ))}
              </tr>
            ))}
          </tbody>
        </table>
      );
      break;
    default:
      content = <div className="content-placeholder">Unsupported file type.</div>;
  }

  return <div className="main-content">{content}</div>;
};

export default MainContent;
