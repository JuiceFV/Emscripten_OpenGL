var navTrigger = document.getElementsByClassName('nav-trigger')[0];
var body = document.getElementsByTagName('body')[0];

console.log(navTrigger);

function toggleNavigation(event) {
  event.preventDefault();
  body.classList.toggle('nav-open');
}

navTrigger.addEventListener('click', toggleNavigation);
