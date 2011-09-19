var timer ;

function construct_table() 
{

    $.ajax({
        url: '/data',
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
        /*
        success: function(data) {
            data = eval(data);
            alert(data);
            var matrix = data.matrix,
                i=0, j=0, row, tt = "";
            alert(matrix);
            tt += "<table>";
            for (i=0; i<matrix.length; i++) {
                row = matrix[i];
                tt += "<tr>";
                for (j=0; j< row.length; j++) {
                    tt += "<td class=";
                    tt += row[j] == 1? "c_black": "c_white"; 
                    tt += "> </td>";
                }
                tt += "</tr>";
            }
            tt += "</table>";
            $('#render').html(tt);   
            timer = setTimeout('construct_table()', 1000);
        }
        */
 
    });
 
}


$(document).ready(function() {
    
    $('#run').click(function() {
        var w = $('#w').val(), 
            h = $('#h').val();
        $.ajax({ url: '/run?height='+h+'&width='+w, 
            success: function(data) {
                construct_table();
            }
        });
    });
});
