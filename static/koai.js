// Previous selections tracking
let currFigher1Record = null;
let currFigher2Record = null;
let controller;
let predController;
let previousFighter1 = '';
let previousFighter2 = '';
let SUGG_LENGTH_TO_SEARCH = 1;

// User section handling
//const userSection = document.getElementById('user-section');

// Fighter 1 Elements
const fighter1 = document.getElementById('fighter1');
const fighter1Img = document.getElementById('fighter1-img');
const fighter1Name = document.getElementById('fighter1-name');
const search1 = document.getElementById('search1');
const searchInput1 = document.getElementById('search-input1');
const suggestions1 = document.getElementById('suggestions1');
const close1 = document.getElementById('close1');

// Fighter 2 Elements
const fighter2 = document.getElementById('fighter2');
const fighter2Img = document.getElementById('fighter2-img');
const fighter2Name = document.getElementById('fighter2-name');
const search2 = document.getElementById('search2');
const searchInput2 = document.getElementById('search-input2');
const suggestions2 = document.getElementById('suggestions2');
const close2 = document.getElementById('close2');

// Prediction Elements
const predictionContainer = document.getElementById('prediction-container');
const predictionText = document.getElementById('prediction-text');
const predictionTextTitle = document.getElementById("prediction-title-text");
const winnerContainer = document.getElementById('winner-container');
const winnerText = document.getElementById('winner-text');

// Open search for Fighter 1
fighter1.addEventListener('click', () => {
    search1.style.display = 'flex';
    searchInput1.focus();
});

// Open search for Fighter 2
fighter2.addEventListener('click', () => {
    search2.style.display = 'flex';
    searchInput2.focus();
});

// Close search for Fighter 1
close1.addEventListener('click', () => {
    search1.style.display = 'none';
});

// Close search for Fighter 2
close2.addEventListener('click', () => {
    search2.style.display = 'none';
});


function debounce(func, delay) {
    let timer;
    return (...args) => {
        clearTimeout(timer);
        timer = setTimeout(() => func(...args), delay);
    };
}

const debouncedFetchSuggestions = debounce(displaySuggestions, 300);

// Handle search input for Fighter 1
searchInput1.addEventListener('input', (e) => {
    const searchValue = e.target.value.toLowerCase();
    if (searchValue.length >= SUGG_LENGTH_TO_SEARCH) {
        debouncedFetchSuggestions(searchValue, suggestions1, 1);
    }
});



// Handle search input for Fighter 2
searchInput2.addEventListener('input', (e) => {
    const searchValue = e.target.value.toLowerCase();
    if (searchValue.length >= SUGG_LENGTH_TO_SEARCH) {
        debouncedFetchSuggestions(searchValue, suggestions2, 2);
    }
});


const debouncedgetPrediction = debounce(getPredition, 200);

// Update prediction text based on selected fighters and check if selection changed
async function updatePrediction() {
    const fighter1Selected = fighter1Name.textContent !== '';
    const fighter2Selected = fighter2Name.textContent !== '';

    const currentFighter1 = fighter1Name.textContent;
    const currentFighter2 = fighter2Name.textContent;

    // Check if at least one selection changed
    const selectionChanged = (currentFighter1 !== previousFighter1) || (currentFighter2 !== previousFighter2);

    if (fighter1Selected && fighter2Selected) {
        // Update previous selections
        previousFighter1 = currentFighter1;
        previousFighter2 = currentFighter2;

        // Only show prediction if the selection changed
        if (selectionChanged) {
            predictionTextTitle.innerText = "Predict...";
            predictionText.innerText = "";
            predictionContainer.classList.add('visible');
            await debouncedgetPrediction();
            await updateRecentPredHistory();
        }
    } else {
        predictionContainer.classList.remove('visible');
        winnerContainer.classList.remove('visible');
        winnerText.classList.remove('visible');
    }

}

//get suggestions
async function getSuggestions(athleteName) {
    /*if (controller) {
      controller.abort();
    }
    controller = new AbortController();
    const signal = controller.signal;*/

    try {
        const response = await fetch('/athlete_search', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                "q": athleteName
            })
        });

        const data = await response.json();
        //const timestamp = new Date().toLocaleTimeString();
        //document.getElementById('statusMessage').prepend(`[${timestamp}] ${data.msg}\n`);
        return data;
    } catch (error) {
        //const timestamp = new Date().toLocaleTimeString();
        //document.getElementById('statusMessage').prepend(`[${timestamp}] Error: ${error.message}\n`);
        console.log(error);
    }
    return {};
}



//get prediction

//get suggestions
async function getPredition() {
    if (predController) {
        predController.abort();
    }
    predController = new AbortController();
    const signal = predController.signal;
    if (currFigher1Record == null || currFigher2Record == null) {
        alert("please select 2 fighters.");
        return;
    }

    let fightersPayload = [currFigher1Record, currFigher2Record];

    try {
        const response = await fetch('/predict', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                "q": fightersPayload
            }),
            signal
        });

        const data = await response.json();
        predictionTextTitle.innerText = "Fight Prediction";
        let preText = data.text;
        //if no prediction comes, change previous fighters to null to allow predicting again.
        if (preText.length == 0 || preText.toLowerCase().includes("sorry")) {
            previousFighter1 = '';
            previousFighter2 = '';
        }
        predictionText.innerText = preText;
    } catch (error) {
        //const timestamp = new Date().toLocaleTimeString();
        //document.getElementById('statusMessage').prepend(`[${timestamp}] Error: ${error.message}\n`);
        console.log(error);
    }
}



//update the recent predictions list

async function updateRecentPredHistory() {
    const historySection = document.querySelector(".history-section");

    try {
        // Fetch data from API (replace with actual API URL)
        const response = await fetch("/recent_pred");
        const data = await response.json();

        // Clear existing history items
        document.querySelectorAll(".history-item").forEach(item => item.remove());
        const firstChild = historySection.firstElementChild;        // Add new matches
        data.forEach(match => {
            const newItem = document.createElement("div");
            newItem.classList.add("history-item");
            newItem.textContent = match
            historySection.insertBefore(newItem, firstChild.nextSibling);
        });
    } catch (error) {
        console.error("Error fetch recent predictions:", error);
    }
}



// Display suggestions based on search
async function displaySuggestions(searchValue, container, fighterNumber) {
    fightersSuggestions = await getSuggestions(searchValue);
    container.innerHTML = '';

    fightersSuggestions.forEach(fighter => {
        const item = document.createElement('div');
        item.className = 'suggestion-item';

        const img = document.createElement('img');
        img.src = fighter.image_url;
        img.className = 'suggestion-img';
        img.alt = fighter.gender;

        const name = document.createElement('div');
        name.textContent = fighter.first_name + " " + fighter.last_name;

        item.appendChild(img);
        item.appendChild(name);

        item.addEventListener('click', () => {
            let nickName = fighter.nickname.length > 0 ? " ( " + fighter.nickname + " ) " : " ";
            if (fighterNumber === 1) {
                searchInput1.textContent = "";
                currFigher1Record = fighter;
                fighter1Img.src = fighter.image_url;
                fighter1Name.textContent = fighter.first_name + nickName + fighter.last_name;
                search1.style.display = 'none';
            } else {
                searchInput2.textContent = "";
                currFigher2Record = fighter;
                fighter2Img.src = fighter.image_url;
                fighter2Name.textContent = fighter.first_name + nickName + fighter.last_name;
                search2.style.display = 'none';
            }
            // Update prediction when a fighter is selected
            updatePrediction();
        });

        container.appendChild(item);
    });

    if (fightersSuggestions.length === 0) {
        const noResults = document.createElement('div');
        noResults.textContent = 'No fighters found';
        noResults.style.padding = '10px';
        noResults.style.textAlign = 'center';
        noResults.style.color = 'var(--text-color)';
        container.appendChild(noResults);
    }
}