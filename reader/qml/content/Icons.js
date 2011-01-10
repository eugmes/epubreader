var previousPageIcon;
var nextPageIcon;
var showTocIcon;
var showFullSizeIcon;
var showNormalIcon;

if (runOnMaemo) {
    previousPageIcon = "qgn_back_fsm";
    nextPageIcon = "qgn_forward_fsm";
    showTocIcon = "notes_bullets";
    showFullSizeIcon = "general_fullsize";
    showNormalIcon = "general_fullsize_b";
} else {
    previousPageIcon = "go-previous";
    nextPageIcon = "go-next";
    showTocIcon = "format-justify-fill";
    showFullSizeIcon = "view-fullscreen";
    showNormalIcon = "view-restore";
}
