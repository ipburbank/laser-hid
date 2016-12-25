/*
* This library numbers figures sequentially in a webpage and edits references to
* them to include their number.
*
* Insert a figure like:
*
* <div class="figure" id="arbitrary_unique_name">
*   <img ... />
*   <div class="caption">Caption for image</div>
* </div>
*
* Then link to it with:
*
* <a class="figref" href="#arbitrary_unique_name">*ref title of link</a>
*
* '*ref' is replaced with "Fig n" where n is the assigned figure number. '*ref'
* can be excluded, in which case the assigned figure number will not be
* mentioned but the link will otherwise behave the same.
*/

function referenced_id(link_elem) {
    return link_elem.href.split("#")[1];
}

function caption_from_figure(fig_elem) {
    return fig_elem.getElementsByClassName("caption")[0];
}

function modify_caption(num, fig_elem) {
    const prefix = "Fig " + num + ": ";
    fig_elem.innerHTML = prefix + fig_elem.innerHTML;
}

// for every figure, prefix the caption with a number and return a map from
// figure id to an object mapping num to the figure number and fig_elem to the
// figure DOM element
function number_figures() {
    let fig_list = document.getElementsByClassName("figure");
    let figs = {};
    // I tried to forEach over this and it didn't work. It is an HTMLCollection.
    for (let i = 0; i < fig_list.length; i++) {
        var elem = fig_list.item(i);
        const fig_num = i + 1
        figs[elem.id] = {num: fig_num,
                         fig_elem: elem};
        const cap = caption_from_figure(elem);
        modify_caption(fig_num, cap);
    }
    return figs;
}

function rename_figlinks(figs) {
    const link_list = document.getElementsByClassName("figref");
    for (let i = 0; i < link_list.length; i++) {
        let link_elem = link_list.item(i);
        const fig_obj = figs[referenced_id(link_elem)];
        const ref_string = "Fig " + fig_obj.num;
        link_elem.innerHTML = link_elem.innerHTML.replace("*ref", ref_string);
    }
}

function match_figure_references() {
    const figs = number_figures();
    rename_figlinks(figs);
}
