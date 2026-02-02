const DIFFICULTIES = {
    easy:   { rows: 9,  cols: 9,  mines: 10 },
    medium: { rows: 16, cols: 16, mines: 40 },
    hard:   { rows: 16, cols: 30, mines: 99 }
};

let currentDifficulty = 'easy';
let ROWS = 9;
let COLS = 9;
let MINES = 10;

let board = [];
let gameOver = false;
let gameWon = false;
let firstClick = true;
let timerInterval = null;
let timeElapsed = 0;
let minesLeft = MINES;

const boardEl = document.getElementById('board');
const minesCountEl = document.getElementById('mines-count');
const timerEl = document.getElementById('timer');
const faceBtn = document.getElementById('face-btn');
const modalOverlay = document.getElementById('modal-overlay');
const modalTime = document.getElementById('modal-time');
const playerNameInput = document.getElementById('player-name');
const submitScoreBtn = document.getElementById('submit-score');

function setDifficulty(level) {
    const diff = DIFFICULTIES[level];
    if (!diff) return;
    currentDifficulty = level;
    ROWS = diff.rows;
    COLS = diff.cols;
    MINES = diff.mines;

    document.querySelectorAll('.diff-btn').forEach(btn => {
        btn.classList.toggle('active', btn.dataset.diff === level);
    });

    initGame();
}

function initGame() {
    gameOver = false;
    gameWon = false;
    firstClick = true;
    timeElapsed = 0;
    minesLeft = MINES;
    clearInterval(timerInterval);
    timerInterval = null;

    board = [];
    for (let r = 0; r < ROWS; r++) {
        board[r] = [];
        for (let c = 0; c < COLS; c++) {
            board[r][c] = { mine: false, revealed: false, flagged: false, adjacentMines: 0 };
        }
    }

    faceBtn.textContent = '\u{1F60A}';
    updateMinesCount();
    updateTimer();
    renderBoard();
    fetchLeaderboard();
}

function placeMines(excludeR, excludeC) {
    let placed = 0;
    while (placed < MINES) {
        const r = Math.floor(Math.random() * ROWS);
        const c = Math.floor(Math.random() * COLS);
        if (board[r][c].mine) continue;
        if (Math.abs(r - excludeR) <= 1 && Math.abs(c - excludeC) <= 1) continue;
        board[r][c].mine = true;
        placed++;
    }
    for (let r = 0; r < ROWS; r++) {
        for (let c = 0; c < COLS; c++) {
            if (board[r][c].mine) continue;
            let count = 0;
            for (let dr = -1; dr <= 1; dr++) {
                for (let dc = -1; dc <= 1; dc++) {
                    const nr = r + dr, nc = c + dc;
                    if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && board[nr][nc].mine) count++;
                }
            }
            board[r][c].adjacentMines = count;
        }
    }
}

function reveal(r, c) {
    if (gameOver || gameWon) return;
    const cell = board[r][c];
    if (cell.revealed || cell.flagged) return;

    if (firstClick) {
        firstClick = false;
        placeMines(r, c);
        startTimer();
    }

    cell.revealed = true;

    if (cell.mine) {
        gameOver = true;
        faceBtn.textContent = '\u{1F635}';
        clearInterval(timerInterval);
        revealAllMines(r, c);
        renderBoard();
        return;
    }

    if (cell.adjacentMines === 0) {
        for (let dr = -1; dr <= 1; dr++) {
            for (let dc = -1; dc <= 1; dc++) {
                const nr = r + dr, nc = c + dc;
                if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS) {
                    reveal(nr, nc);
                }
            }
        }
    }

    if (checkWin()) {
        gameWon = true;
        faceBtn.textContent = '\u{1F60E}';
        clearInterval(timerInterval);
        modalTime.textContent = timeElapsed;
        modalOverlay.classList.add('active');
        playerNameInput.value = '';
        playerNameInput.focus();
    }

    renderBoard();
}

function revealAllMines(clickedR, clickedC) {
    for (let r = 0; r < ROWS; r++) {
        for (let c = 0; c < COLS; c++) {
            if (board[r][c].mine) {
                board[r][c].revealed = true;
                board[r][c]._clicked = (r === clickedR && c === clickedC);
            }
        }
    }
}

function toggleFlag(r, c) {
    if (gameOver || gameWon) return;
    const cell = board[r][c];
    if (cell.revealed) return;
    cell.flagged = !cell.flagged;
    minesLeft += cell.flagged ? -1 : 1;
    updateMinesCount();
    renderBoard();
}

function checkWin() {
    for (let r = 0; r < ROWS; r++) {
        for (let c = 0; c < COLS; c++) {
            if (!board[r][c].mine && !board[r][c].revealed) return false;
        }
    }
    return true;
}

function startTimer() {
    timerInterval = setInterval(() => {
        timeElapsed++;
        updateTimer();
    }, 1000);
}

function updateTimer() {
    timerEl.textContent = String(timeElapsed).padStart(3, '0');
}

function updateMinesCount() {
    minesCountEl.textContent = String(minesLeft).padStart(3, '0');
}

function renderBoard() {
    boardEl.style.gridTemplateColumns = `repeat(${COLS}, 30px)`;
    boardEl.style.gridTemplateRows = `repeat(${ROWS}, 30px)`;
    boardEl.innerHTML = '';
    for (let r = 0; r < ROWS; r++) {
        for (let c = 0; c < COLS; c++) {
            const cell = board[r][c];
            const el = document.createElement('div');
            el.className = 'cell';

            if (cell.revealed) {
                if (cell.mine) {
                    el.classList.add('mine');
                    if (cell._clicked) el.classList.add('mine-clicked');
                    el.textContent = '\u{1F4A3}';
                } else {
                    el.classList.add('revealed');
                    if (cell.adjacentMines > 0) {
                        el.textContent = cell.adjacentMines;
                        el.classList.add('num-' + cell.adjacentMines);
                    }
                }
            } else {
                el.classList.add('unrevealed');
                if (cell.flagged) {
                    el.classList.add('flagged');
                }
            }

            el.addEventListener('click', () => reveal(r, c));
            el.addEventListener('contextmenu', (e) => {
                e.preventDefault();
                toggleFlag(r, c);
            });

            boardEl.appendChild(el);
        }
    }
}

async function fetchLeaderboard() {
    try {
        const res = await fetch(`/api/leaderboard?difficulty=${currentDifficulty}`);
        const data = await res.json();
        const tbody = document.getElementById('leaderboard-body');
        tbody.innerHTML = '';
        data.forEach((entry, i) => {
            const tr = document.createElement('tr');
            tr.innerHTML = `<td>${i + 1}</td><td>${escapeHtml(entry.player_name)}</td><td>${entry.time_seconds}s</td>`;
            tbody.appendChild(tr);
        });
    } catch (e) {
        console.error('Failed to fetch leaderboard:', e);
    }
}

async function submitScore() {
    const name = playerNameInput.value.trim();
    if (!name) {
        playerNameInput.focus();
        return;
    }
    try {
        await fetch('/api/leaderboard', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ player_name: name, time_seconds: timeElapsed, difficulty: currentDifficulty })
        });
        modalOverlay.classList.remove('active');
        fetchLeaderboard();
    } catch (e) {
        console.error('Failed to submit score:', e);
    }
}

function escapeHtml(text) {
    const div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML;
}

document.querySelectorAll('.diff-btn').forEach(btn => {
    btn.addEventListener('click', () => setDifficulty(btn.dataset.diff));
});
faceBtn.addEventListener('click', initGame);
submitScoreBtn.addEventListener('click', submitScore);
playerNameInput.addEventListener('keydown', (e) => {
    if (e.key === 'Enter') submitScore();
});

initGame();
