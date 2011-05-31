var timer ;

function construct_table() 
{

    $.ajax({
        url: '/data.lua',
        dataType: 'text/plain',
        success: function(data) {
            var result = data.split("$"),
                i=0, j=0, row, tt = "";
            tt += "<table>";
            for (i=0; i<result.length; i++) {
                row = result[i];
                tt += "<tr>";
                for (j=0; j< row.length; j++) {
                    tt += "<td class=";
                    tt += row[j] == "#"? "c_black": "c_white"; 
                    tt += "> </td>";
                }
                tt += "</tr>";
            }
            tt += "</table>";
            $('#render').html(tt);   
            timer = setTimeout('construct_table()', 1000);
        }
    });
 
}

$(document).ready(function() {
    construct_table();
});
