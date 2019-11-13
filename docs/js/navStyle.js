$(function () {
	var url = window.location.pathname;
	var navLinks = $("#mainNavigation ul li a").toArray();
	for (var i = 0; i < navLinks.length; i++) {
		if (
			navLinks[i].pathname == url ||
			(url == "/" && navLinks[i].pathname == "/index.html")
		) {
			navLinks[i].classList.add("currentPageNav");
		}
	}
});
