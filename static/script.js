let isPolling = false;
let pollInterval = null;

const elements = {
    statusDot: document.getElementById('statusDot'),
    statusText: document.getElementById('statusText'),
    startBtn: document.getElementById('startBtn'),
    stopBtn: document.getElementById('stopBtn'),
    singleBtn: document.getElementById('singleBtn'),
    clearBtn: document.getElementById('clearBtn'),
    downloadBtn: document.getElementById('downloadBtn'),
    csvModal: document.getElementById('csvModal'),
    csvList: document.getElementById('csvList'),
    closeModal: document.querySelector('.close'),
    oxygenValue: document.getElementById('oxygenValue'),
    temperatureValue: document.getElementById('temperatureValue'),
    phaseValue: document.getElementById('phaseValue'),
    amplitudeValue: document.getElementById('amplitudeValue'),
    oxygenBar: document.getElementById('oxygenBar'),
    temperatureBar: document.getElementById('temperatureBar'),
    historyContainer: document.getElementById('historyContainer'),
    lastUpdate: document.getElementById('lastUpdate')
};

async function apiCall(endpoint, method = 'GET') {
    try {
        const response = await fetch(`/api/${endpoint}`, {
            method: method,
            headers: {
                'Content-Type': 'application/json'
            }
        });
        
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        
        return await response.json();
    } catch (error) {
        console.error(`Error calling ${endpoint}:`, error);
        showNotification(`Error: ${error.message}`, 'error');
        return null;
    }
}

function updateDisplay(data) {
    if (!data) return;
    
    elements.oxygenValue.textContent = data.oxygen.toFixed(2);
    elements.temperatureValue.textContent = data.temperature.toFixed(2);
    elements.phaseValue.textContent = data.phase.toFixed(2);
    elements.amplitudeValue.textContent = data.amplitude;
    
    elements.oxygenBar.style.width = Math.min(100, (data.oxygen / 150) * 100) + '%';
    elements.temperatureBar.style.width = Math.min(100, ((data.temperature + 10) / 60) * 100) + '%';
    
    const now = new Date();
    elements.lastUpdate.textContent = now.toLocaleTimeString();
}

function addToHistory(data) {
    if (!data) return;
    
    const time = new Date(data.timestamp);
    const historyItem = document.createElement('div');
    historyItem.className = 'history-item';
    historyItem.innerHTML = `
        <span class="history-time">${time.toLocaleTimeString()}</span>
        <div class="history-values">
            <span class="history-value">O₂: ${data.oxygen.toFixed(2)}%</span>
            <span class="history-value">T: ${data.temperature.toFixed(2)}°C</span>
        </div>
    `;
    
    elements.historyContainer.insertBefore(historyItem, elements.historyContainer.firstChild);
    
    if (elements.historyContainer.children.length > 50) {
        elements.historyContainer.removeChild(elements.historyContainer.lastChild);
    }
}

async function updateStatus() {
    const status = await apiCall('status');
    if (!status) return;
    
    if (status.measuring) {
        elements.statusDot.className = 'status-dot measuring';
        elements.statusText.textContent = 'Measuring';
        elements.startBtn.disabled = true;
        elements.stopBtn.disabled = false;
        elements.singleBtn.disabled = true;
    } else if (status.connected) {
        elements.statusDot.className = 'status-dot connected';
        elements.statusText.textContent = 'Connected';
        elements.startBtn.disabled = false;
        elements.stopBtn.disabled = true;
        elements.singleBtn.disabled = false;
    } else {
        elements.statusDot.className = 'status-dot';
        elements.statusText.textContent = 'Disconnected';
        elements.startBtn.disabled = false;
        elements.stopBtn.disabled = true;
        elements.singleBtn.disabled = false;
    }
    
    if (status.measurements && status.measurements.length > 0) {
        const latest = status.measurements[status.measurements.length - 1];
        updateDisplay(latest);
        
        elements.historyContainer.innerHTML = '';
        status.measurements.slice().reverse().forEach(m => addToHistory(m));
    }
}

function startPolling() {
    if (isPolling) return;
    isPolling = true;
    updateStatus();
    pollInterval = setInterval(updateStatus, 2000);
}

function stopPolling() {
    isPolling = false;
    if (pollInterval) {
        clearInterval(pollInterval);
        pollInterval = null;
    }
}

function showNotification(message, type = 'info') {
    console.log(`[${type.toUpperCase()}] ${message}`);
}

elements.startBtn.addEventListener('click', async () => {
    elements.startBtn.disabled = true;
    const result = await apiCall('start', 'POST');
    
    if (result && result.status === 'started') {
        showNotification('Measurements started', 'success');
        startPolling();
    } else {
        elements.startBtn.disabled = false;
    }
});

elements.stopBtn.addEventListener('click', async () => {
    elements.stopBtn.disabled = true;
    const result = await apiCall('stop', 'POST');
    
    if (result && result.status === 'stopped') {
        showNotification('Measurements stopped', 'success');
        await updateStatus();
    }
    elements.stopBtn.disabled = false;
});

elements.singleBtn.addEventListener('click', async () => {
    elements.singleBtn.disabled = true;
    const result = await apiCall('measure', 'POST');
    
    if (result && result.status === 'success') {
        updateDisplay(result.data);
        addToHistory(result.data);
        showNotification('Measurement completed', 'success');
    }
    
    elements.singleBtn.disabled = false;
});

elements.clearBtn.addEventListener('click', async () => {
    const result = await apiCall('clear', 'POST');
    if (result && result.status === 'cleared') {
        elements.historyContainer.innerHTML = '';
        showNotification('History cleared', 'success');
    }
});

async function loadCSVFiles() {
    const result = await apiCall('list-csv');
    if (result && result.status === 'success') {
        elements.csvList.innerHTML = '';
        
        if (result.files.length === 0) {
            elements.csvList.innerHTML = '<p style="text-align: center; color: #666;">No CSV files available</p>';
            return;
        }
        
        result.files.forEach(file => {
            const item = document.createElement('div');
            item.className = 'csv-item';
            
            const sizeKB = (file.size / 1024).toFixed(1);
            
            item.innerHTML = `
                <div class="csv-info">
                    <div class="csv-name">${file.filename}</div>
                    <div class="csv-meta">${file.readable_date} · ${sizeKB} KB</div>
                </div>
                <button class="csv-download" data-filename="${file.filename}">
                    Download
                </button>
            `;
            
            item.querySelector('.csv-download').addEventListener('click', (e) => {
                e.stopPropagation();
                downloadCSV(file.filename);
            });
            
            elements.csvList.appendChild(item);
        });
    }
}

function downloadCSV(filename) {
    const link = document.createElement('a');
    link.href = `/api/download-csv/${filename}`;
    link.download = filename;
    document.body.appendChild(link);
    link.click();
    document.body.removeChild(link);
    showNotification(`Downloading ${filename}...`, 'info');
}

elements.downloadBtn.addEventListener('click', async () => {
    await loadCSVFiles();
    elements.csvModal.classList.add('show');
});

elements.closeModal.addEventListener('click', () => {
    elements.csvModal.classList.remove('show');
});

elements.csvModal.addEventListener('click', (e) => {
    if (e.target === elements.csvModal) {
        elements.csvModal.classList.remove('show');
    }
});

startPolling();
updateStatus();