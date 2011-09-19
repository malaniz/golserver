/* TODO:
 * - ufff, very very best design of the code.
 * - widgets? use some of eonblast
 * - better laymut interface, use stack and grid layout?
 *   - performance? to much work?
 * .
 * .
 * - Planed refactoring in 21, 22, 23 of march
 */
(function() {
    post_functions = [];
    var FormPhoto, FormProduct, form, render_listing,
        id='test';

    render_listing = function () {
        var _pid = 1;
        $.ajax({ 
            url: "/products.json?user="+ id, 
            async: false,
            success : function(data) {
                var tmp = JSON.parse(data),
                    products = tmp.data,
                    prod = null,
                    pid = 0,
                    div_list = $('#render-listing');

                for( pid in products) {
                    prod = products[pid];
                    $(div_list).append("<div id='update_form_" + pid + "'" + 
                        "class='span-22 last'></div>");

                    form = new FormProduct({
                        id: id,
                        pid: pid,
                        description: prod.description,
                        thumb: prod.thumb,
                        price: prod.price,
                        container: $('#update_form_' + pid) 
                    });
                    form.render();
                } 
                _pid = pid;
            }
        });
        return _pid;
    };
 

    FormPhoto = function(params) {
        var container = "";
        this.open = function() {
            $(this.container).dialog('open')
        };
        this.close = function() {
            $(this.container).dialog('close')
        };
 
        this.before = function() {
            var id = $(this.container).attr('id'); 
            $("#" + id + " form").submit(function(e) {
                var   values = $(this).serialize();

                $.ajax({ type: "POST", url: "/upload",  
                    data: values, dataType: "json",
                    success: function ( data ) {
                        alert(JSON.stringify(data));
                    },
                    error: function ( msg ) {
                        alert("error");
                    },
                });
                return true;
            });
        }
        this.render = function(container) {
            var html = "";
            html += '<form action="/upload" method="post" enctype="multipart/form-data" >' ;
            html += '<img src="/images/products/defaultproduct.jpg"><br>';
            html += 'Image <input type="file" name="file" /><br>' ;
            html += '<input type="submit" value="UpLoad" />' ;
            html += '</form>' ;
            $(container).html(html).attr('title', 'UpLoad a product Image')
                .dialog({ autoOpen: false, modal:true, 
                          width: 500, height: 400});
            this.container = container;
            this.before();
        };
    }

    FormProduct = function(params) {
        this.callbacks = [];
        this.id = params.id || false;
        this.pid = params.pid || false;
        this.thumb = params.thumb || "/images/products/defaultproduct.jpg";    
        this.description = params.description || 
                           "Please write your product's description here.";
        this.price = params.price || "0.00";
        this.container = params.container;
        this.listing = params.listing;
        this.dialogPhoto = new FormPhoto();

        var self = this;

        this.update_post = function () {
            $('#form_' + this.pid).submit(function() {
                var old_form = $(this).html(),
                    values = $(this).serialize();
                $(this).html("<div class='span-22 last'>"+
                    "<center> <img src='/images/ajax.gif'></center></div>");

                $.ajax({ type: "POST", url: "/products", async: false, 
                    data: values, dataType: "json",
                    success: function ( data ) {
                        var p = data.data;
                        self.thumb = p.thumb;
                        self.description = p.description;
                        self.name = p.name;
                        self.price = p.price;
                        self.render();
                    },
                    error: function ( msg ) {
                        $(self).html("<div class='ui-state-error'>" + 
                            msg + "</div>");
                    },
                });
                return false;
            });
        };   

        this.new_post = function () {
            $('#form_' + this.pid).submit(function() {
                var old_form = $(this).html(),
                    values = $(this).serialize();
                $(this).html("<div class='span-22 last'>"+
                    "<center> <img src='/images/ajax.gif'></center></div>");

                $.ajax({ type: "POST", url: "/products", async: false, 
                    data: values, dataType: "json",
                    success: function ( data ) {
                        var p = data.data;
                        self.pid = parseInt(render_listing())+1;
                        self.thumb = "/images/products/defaultproduct.jpg";    
                        self.description = "";
                        self.name = "";
                        self.price = "0.00";
                        //self.reset();
                        self.render();
                    },
                    error: function ( msg ) {
                        $(self).html("<div class='ui-state-error'>" + 
                            msg + "</div>");
                    },
                });
                return false;
            });
        };   

        this.string_html = function () {
            var html = "";
            html += '<form method="post" action="" id="form_'+this.pid+'">';
            html += '<div class="span-22 last">';
            html += '<div class="span-3">';
            html += '<a href="" id="open_photo_'+ this.pid+'">';
            html += '<img src="' + this.thumb + '" class="thumb">';
            html += '</a>';
            html += '</div>';
            html += '<div class="span-18 last">';
            html += '<textarea name="description" ' +
                    'class="ui-corner-all span-18 last">';
            html += this.description + '</textarea>';
            html += '<input type="text" name="price" ' +
                    'class="ui-corner-all span-3" value="' + this.price + '" />';
            html += '<input type="hidden" name="uid" value="'+ this.id +'" >'; 
            html += '<input type="hidden" name="pid" value="'+ this.pid +'" >'; 
            html += '<input type="submit" id="sub_' +this.pid +'"' + 
                    'class="ui-corner-all" value="Save" />';
            html += '</div>';
            html += '</div>';
            html += '</form>';
            html += '<div id="dialog_photo_' + this.pid + '" >.';
            html += '</div>';
            return html;
        };

        this.render = function (  ) {
            var dialog = this.dialogPhoto;
            $(this.container).html(this.string_html());
            
            $('#sub_'+this.pid).button(); 

            if (this.listing) { 
                this.new_post();    
            } else {
                this.update_post(); 
            }

            dialog.render($('#dialog_photo_'+this.pid));

            $('#open_photo_' + this.pid).click(function(e) {
              e.preventDefault();
              dialog.open();
            });
        };
    };
    
    var last_pid = parseInt(render_listing())+1;
    form = new FormProduct({
        id: id, pid: last_pid, 
        container: $('#render-newproduct'), 
        listing: $('#render-listing')
    });

    form.render();
     
})();
