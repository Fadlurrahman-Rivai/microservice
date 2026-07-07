/**
 * MarketPlace – API Configuration & Shared Utilities
 * Semua komunikasi ke microservices dilakukan melalui file ini.
 */

'use strict';

// ── API Base URLs ─────────────────────────────────────────────────
// Sesuaikan port jika .env diubah
const API = {
    CATALOG:   'http://localhost:8001',
    CART:      'http://localhost:8002',
    INVENTORY: 'http://localhost:8003',
    PAYMENT:   'http://localhost:8004',
};

// Ambil user ID dari localStorage (dari auth.js), default ke 1 jika belum login
const USER_ID = (() => {
    try {
        const user = JSON.parse(localStorage.getItem('user') || 'null');
        return user && user.id ? user.id : 1;
    } catch (e) {
        return 1;
    }
})();

// ── HTTP Helper ───────────────────────────────────────────────────
/**
 * Wrapper fetch dengan Content-Type JSON dan error handling bawaan.
 * Throws Error jika response tidak ok.
 *
 * @param {string} url
 * @param {RequestInit} [options]
 * @returns {Promise<any>}
 */
async function apiCall(url, options = {}) {
    const cfg = {
        headers: { 'Content-Type': 'application/json', 'Accept': 'application/json' },
        ...options,
    };
    if (options.headers) {
        cfg.headers = { ...cfg.headers, ...options.headers };
    }

    const response = await fetch(url, cfg);

    if (!response.ok) {
        let errMsg = `HTTP ${response.status}`;
        try {
            const body = await response.json();
            errMsg = body.error || errMsg;
        } catch (_) { /* json parse failed */ }
        const err = new Error(errMsg);
        err.status = response.status;
        throw err;
    }

    return response.json();
}

// ── Formatters ────────────────────────────────────────────────────
function formatCurrency(amount) {
    return new Intl.NumberFormat('id-ID', {
        style: 'currency', currency: 'IDR', minimumFractionDigits: 0,
    }).format(Number(amount));
}

function formatDate(dateStr) {
    return new Date(dateStr).toLocaleString('id-ID', {
        dateStyle: 'medium', timeStyle: 'short',
    });
}

// ── Stock Badge ───────────────────────────────────────────────────
/**
 * Hasilkan HTML badge stok untuk kartu produk.
 * @param {number|null} stok  null = data tidak tersedia
 */
function buildStockBadge(stok) {
    if (stok === null || stok === undefined) return '';
    if (stok === 0)  return '<span class="badge bg-danger"><i class="bi bi-x-circle me-1"></i>Stok Habis</span>';
    if (stok <= 5)   return `<span class="badge bg-warning text-dark"><i class="bi bi-exclamation-triangle me-1"></i>Tersisa ${stok}</span>`;
    return `<span class="badge bg-success"><i class="bi bi-check-circle me-1"></i>Stok: ${stok}</span>`;
}

// ── Alert Helper ──────────────────────────────────────────────────
function showAlert(message, type = 'info', containerId = 'alert-container') {
    const el = document.getElementById(containerId);
    if (!el) return;
    const id = 'alert-' + Date.now();
    el.innerHTML = `
        <div id="${id}" class="alert alert-${type} alert-dismissible fade show shadow-sm" role="alert">
            ${message}
            <button type="button" class="btn-close" data-bs-dismiss="alert" aria-label="Tutup"></button>
        </div>`;
    setTimeout(() => document.getElementById(id)?.remove(), 6000);
}

// ── Loading Spinner ───────────────────────────────────────────────
function showLoading(containerId, message = 'Memuat data...') {
    const el = document.getElementById(containerId);
    if (!el) return;
    el.innerHTML = `
        <div class="d-flex justify-content-center align-items-center py-5">
            <div class="spinner-border text-primary me-3" role="status"></div>
            <span class="text-muted">${message}</span>
        </div>`;
}

// ── Cart Badge ────────────────────────────────────────────────────
function updateCartBadge(count) {
    localStorage.setItem('mp_cart_count', String(count));
    document.querySelectorAll('.cart-badge').forEach(el => {
        el.textContent = count;
        el.style.display = count > 0 ? 'inline-flex' : 'none';
    });
}

async function refreshCartBadge() {
    try {
        const data = await apiCall(`${API.CART}/cart?user_id=${USER_ID}`);
        updateCartBadge(data.data.item_count ?? 0);
    } catch (_) {
        const cached = parseInt(localStorage.getItem('mp_cart_count') || '0', 10);
        updateCartBadge(cached);
    }
}

// ── Active Nav ────────────────────────────────────────────────────
function setActiveNav() {
    const page = location.pathname.split('/').pop() || 'index.html';
    document.querySelectorAll('.nav-link').forEach(link => {
        const href = link.getAttribute('href') || '';
        if (href === page || (page === '' && href === 'index.html')) {
            link.classList.add('active');
            link.setAttribute('aria-current', 'page');
        }
    });
}

// ── Init on DOM ready ─────────────────────────────────────────────
document.addEventListener('DOMContentLoaded', () => {
    refreshCartBadge();
    setActiveNav();
});
