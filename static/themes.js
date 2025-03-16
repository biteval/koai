// Set current year in footer
document.getElementById('current-year').textContent = new Date().getFullYear();


// Theme handling
const themeToggleCheckbox = document.getElementById('theme-toggle-checkbox');
const mobileThemeToggle = document.getElementById('mobile-theme-toggle');

// Check for saved theme preference
const savedTheme = localStorage.getItem('theme');
if (savedTheme === 'dark') {
    document.body.classList.add('dark-theme');
    themeToggleCheckbox.checked = true;
}

// Theme toggle event listeners
themeToggleCheckbox.addEventListener('change', function () {
    document.body.classList.toggle('dark-theme');
    const currentTheme = document.body.classList.contains('dark-theme') ? 'dark' : 'light';
    localStorage.setItem('theme', currentTheme);
});

mobileThemeToggle.addEventListener('click', function () {
    document.body.classList.toggle('dark-theme');
    themeToggleCheckbox.checked = document.body.classList.contains('dark-theme');
    const currentTheme = document.body.classList.contains('dark-theme') ? 'dark' : 'light';
    localStorage.setItem('theme', currentTheme);
});

// Sidebar elements
const sidebar = document.getElementById('sidebar');
const sidebarActivator = document.getElementById('sidebar-activator');
const mobileSidebarToggle = document.getElementById('mobile-sidebar-toggle');
const container = document.querySelector('.container');

// Sidebar activation on hover
sidebarActivator.addEventListener('mouseenter', function () {
    sidebar.classList.add('visible');
});

// Sidebar stays visible when hovering on it
sidebar.addEventListener('mouseenter', function () {
    sidebar.classList.add('visible');
});

// Sidebar hides when mouse leaves both activator and sidebar
sidebar.addEventListener('mouseleave', function (e) {
    sidebar.classList.remove('visible');
});

// Mobile sidebar toggle
mobileSidebarToggle.addEventListener('click', function () {
    sidebar.classList.toggle('visible');
});

// Close sidebar when clicking outside
document.addEventListener('click', function (event) {
    if (!sidebar.contains(event.target) && !mobileSidebarToggle.contains(event.target)) {
        sidebar.classList.remove('active');
    }
});