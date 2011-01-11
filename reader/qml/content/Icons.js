var previousPageIcon;
var nextPageIcon;
var showTocIcon;
var showFullSizeIcon;
var showNormalIcon;
var unknownBookIcon;

if (runOnMaemo) {
    previousPageIcon = "qgn_back_fsm";
    nextPageIcon = "qgn_forward_fsm";
    showTocIcon = "notes_bullets";
    showFullSizeIcon = "general_fullsize";
    showNormalIcon = "general_fullsize_b";
    unknowBookIcon = "filemanager_unknown_file";
} else {
    previousPageIcon = "go-previous";
    nextPageIcon = "go-next";
    showTocIcon = "format-justify-fill";
    showFullSizeIcon = "view-fullscreen";
    showNormalIcon = "view-restore";
    unknownBookIcon = "unknown";
}
