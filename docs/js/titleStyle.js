function resizeTitle() {
	var title = $(".title")[0];
	if (window.innerWidth <= 417) {
		title.classList.add("smallerTitle");
	} else {
		title.classList.remove("smallerTitle");
	}
}
