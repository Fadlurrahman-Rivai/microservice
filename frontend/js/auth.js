/**
 * MarketPlace – Authentication & Authorization Management
 * Mengelola login, logout, dan role-based access control
 */

'use strict';

/**
 * Fungsi untuk mendapatkan user yang sedang login
 * @returns {Object|null} User object atau null jika tidak login
 */
function getCurrentUser() {
  const userJson = localStorage.getItem('user');
  return userJson ? JSON.parse(userJson) : null;
}

/**
 * Fungsi untuk mengecek apakah user adalah admin
 * @returns {boolean}
 */
function isAdmin() {
  const user = getCurrentUser();
  return user && user.role === 'admin';
}

/**
 * Fungsi untuk mengecek apakah user sudah login
 * @returns {boolean}
 */
function isLoggedIn() {
  return localStorage.getItem('user') !== null;
}

/**
 * Fungsi untuk logout
 */
function logout() {
  if (confirm('Apakah Anda yakin ingin logout?')) {
    localStorage.removeItem('user');
    localStorage.removeItem('cart');
    window.location.href = 'login.html';
  }
}

/**
 * Fungsi untuk mengarahkan ke login jika belum login
 * @param {boolean} adminOnly - Jika true, hanya admin yang boleh akses
 */
function requireLogin(adminOnly = false) {
  const user = getCurrentUser();
  
  if (!user) {
    alert('Anda harus login terlebih dahulu');
    window.location.href = 'login.html';
    return false;
  }

  if (adminOnly && !isAdmin()) {
    alert('Anda tidak memiliki akses ke halaman ini. Hanya admin yang dapat mengaksesnya.');
    window.location.href = 'index.html';
    return false;
  }

  return true;
}

/**
 * Fungsi untuk menampilkan info user di navbar
 */
function updateNavbarUserInfo() {
  const user = getCurrentUser();
  const userInfoContainer = document.getElementById('user-info-container');
  
  if (!userInfoContainer) return;

  if (user) {
    const badgeClass = user.role === 'admin' ? 'bg-danger' : 'bg-success';
    const roleText = user.role === 'admin' ? 'Admin' : 'Pengunjung';
    
    userInfoContainer.innerHTML = `
      <div class="d-flex align-items-center gap-2">
        <span class="badge ${badgeClass}">${roleText}</span>
        <span class="text-light me-2">${user.username}</span>
        <button class="btn btn-outline-light btn-sm" onclick="logout()">
          <i class="bi bi-box-arrow-right me-1"></i>Logout
        </button>
      </div>
    `;
  } else {
    userInfoContainer.innerHTML = `
      <a href="login.html" class="btn btn-light btn-sm">
        <i class="bi bi-box-arrow-in-right me-1"></i>Login
      </a>
    `;
  }
}

/**
 * Fungsi untuk menyembunyikan menu admin jika user bukan admin
 */
function updateAdminMenuVisibility() {
  const adminMenu = document.getElementById('admin-menu');
  if (adminMenu) {
    adminMenu.style.display = isAdmin() ? 'block' : 'none';
  }

  // Riwayat transaksi hanya untuk admin.
  document.querySelectorAll('a[href="transactions.html"]').forEach(link => {
    const wrapper = link.closest('.nav-item') || link;
    wrapper.style.display = isAdmin() ? '' : 'none';
  });
}

/**
 * Guard halaman agar login menjadi gerbang utama sebelum script halaman dieksekusi.
 */
function enforcePageAccess() {
  const currentPage = (window.location.pathname.split('/').pop() || 'index.html').toLowerCase();

  if (currentPage === 'login.html') return true;
  if (currentPage === 'admin.html' || currentPage === 'transactions.html') return requireLogin(true);
  return requireLogin(false);
}

enforcePageAccess();

/**
 * Inisialisasi auth pada saat halaman dimuat
 */
document.addEventListener('DOMContentLoaded', () => {
  if (!enforcePageAccess()) return;

  updateNavbarUserInfo();
  updateAdminMenuVisibility();
});
