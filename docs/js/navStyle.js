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

function resizeNavBar() {
	var navBar = $("#mainNavigation ul")[0];
	var navBarItems = $("#mainNavigation ul li");
	if (navBar.offsetWidth < 1205) {
		// Nav bar is on 2+ lines
		var itemsOnSecondLine = 5 - parseInt((navBar.offsetWidth - 200) / 201);
		if (itemsOnSecondLine < 4) {
			// 2 lines specificially
			for (var i = 0; i < navBarItems.length; i++) {
				if (i == 6 - itemsOnSecondLine) {
					navBarItems[i].classList.add("navNewLineFirst");
				} else {
					navBarItems[i].classList.remove("navNewLineFirst");
				}

				if (i >= 6 - itemsOnSecondLine) {
					navBarItems[i].classList.add("navNotTopRow");
				} else {
					navBarItems[i].classList.remove("navNotTopRow");
				}

			}
		} else {
			// 3+ lines
			if (navBar.offsetWidth < 400) {
				// 5 lines (since 4 is not possible)
				for (var i = 0; i < navBarItems.length; i++) {
					navBarItems[i].classList.add("navNewLineFirst");
					navBarItems[i].classList.remove("navSpecialCase");
					if (i >= 1) navBarItems[i].classList.add("navNotTopRow");
				}
			} else if (navBar.offsetWidth == 400) {
				// Awkward case, since removing the formatting gives this one odd case
				for (var i = 0; i < navBarItems.length; i++) {
					navBarItems[i].classList.add("navSpecialCase");
					navBarItems[i].classList.add("navNewLineFirst");
					if (i >= 1) navBarItems[i].classList.add("navNotTopRow");
				}
			} else {
				// 3 lines
				for (var i = 0; i < navBarItems.length; i++) {

					navBarItems[i].classList.remove("navSpecialCase");

					if (i % 2 == 0) {
						navBarItems[i].classList.add("navNewLineFirst");
					} else {
						navBarItems[i].classList.remove("navNewLineFirst");
					}

					if (i >= 2) {
						navBarItems[i].classList.add("navNotTopRow");
					} else {
						navBarItems[i].classList.remove("navNotTopRow");
					}
				}

			}
		}
	} else {
		// Entire nav bar is on 1 line
		for (var i = 0; i < navBarItems.length; i++) {
			navBarItems[i].classList.remove("navNewLineFirst");
			navBarItems[i].classList.remove("navNotTopRow");
		}
	}
}
