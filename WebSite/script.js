$(document).ready(function(){
    var previous = $("null");
    var hasClicked = false;
    var numBox = 128;
    var color;
    var data_color = "BLACK";
    var mouseDown;
    var numCols = 16;
    var patternArray = [];
   
/*----------------------------------------------------------------
                     TRACK BOX SET-UP
------------------------------------------------------------------*/

    $(".track_box").each(function(){
	$(this).css('background-color','#000000');
    });
    
/*-----------------------------------------------------------------
                       MOUSE AWARENESS
-------------------------------------------------------------------*/
 
    $(document).mousedown(function(){
	    mouseDown = true;
    });
    
    $(document).mouseup(function(){
	    mouseDown = false;
    });
    
    /*-------------------------------------------------------------
      COLOR BOX ADJUSTMENTS
      ----------------------------------------------------------------*/    

    // keeps track of color selected from color container
    $(".color_box").each(function(){
	    $(this).click(function(){
	        
            if(hasClicked)
	        {
		        previous.css('border', '1px solid #474A57');
		        previous.css('margin', '4px');
	        }
	        else
	        {
		        hasClicked = true;
	        }
	        
	        $(this).css('margin','2px');	   
	        $(this).css('border','3px solid white');	    
	        
	        previous =  $(this);
	        color = $(this).css("background-color");
	        data_color = $(this).attr('id');
	    });
    });
    
    /*---------------------------------------------------------------------------
      COLORING THE TRACK
      -----------------------------------------------------------------------------*/    

    // colors the track with the selected color 
    $('.track_box').each(function(){
        
        $(this).mouseenter(function(){
   	        if(mouseDown)
	        {
	            $(this).css('background-color',color); // changes color of box         
	            $(this).data("color", data_color);    
	        }  
        });

	    $(this).mousedown(function(){
	        $(this).css('background-color',color); 
	        $(this).data("color", data_color);     
	    });
        
	    $(this).click(function(){
	        $(this).css('background-color',color);    //changes color of box        
	        $(this).data("color", data_color);    
	    });
    });
	

    /*----------------------------------------------------------------------------
      RESET
      ------------------------------------------------------------------------------*/  

    // resets the track to black when reset button is clicked
    $('#resetColors').click(function(){

	    
	    // RESET UDELAY TEXT BOXES
	    
	    $('.track_box').each(function(){
	        $(this).css('background-color','#000000'); 
	        $(this).data("color", "BLACK");     
	    });

	    $('#patternCode').empty();
    });

    /*------------------------------------------------------------------------------   
      SUBMIT
      --------------------------------------------------------------------------------*/

    $('#submit').click(function(){
        
        var value;
        var byteCount = 0;
        
        $('#patternCode').empty();
        
        $('#patternCode').append("<p>");

        for ( i = 0; i < numCols; i++) {
	        
	        value = document.getElementById('udtbox' + i).value;

   	        $('#patternCode').append( "MS" + value  + ", <br />");
	        
	        for ( j = 7; j >= 0 ; j--) {
	            $('#patternCode').append( $('#box' + (i*8 + j)).data("color") + ", <br />");
	        }    

	        $('#patternCode').append("<br />");
	        byteCount += 9;
        }
        
        $('#patternCode').append("RESET <br /><br />");

        byteCount++;
        $('#patternCode').append("BYTECOUNT: " + byteCount);

        $('#patternCode').append("</p>");
    });

    /*--------------------------------------------------------------------------------------

      ADD COLUMNS
      ---------------------------------------------------------------------------------------*/



    $('#add_columns').click(function(){


	    if(numCols < 33)
	    {
	        var width = parseInt($('#track_sect_holder').css('width'));


	        // Track Columns
	        $('#track_sect_holder').css('width', (width + 55) + "px");
	        $('#track_sect_holder').append('<div class="track_section" id="section'+ numCols +'"></div>');

	        
	        for (i = 0; i < 8; i++)
	        {
	            $('#section' + numCols).append('<div class="track_box" id="box' + numBox +'" data-color="BLACK"></div>');		
		        numBox++;
	        } 
	        
	        
	        $('#section' + numCols).children().bind('click', function(){
		        $(this).css('background-color',color);    //changes color of box
		        $(this).data("color", data_color);    
	        });

	        $('#section' + numCols).children().bind('mouseenter', function(){
		        if(mouseDown)
		        {
		            $(this).css('background-color',color);    //changes color of box
		            $(this).data("color", data_color);    
		        }
	        });
	        
	        $('#section' + numCols).children().bind('mousedown', function(){
		        
		        $(this).css('background-color',color);    //changes color of box
		        $(this).data("color", data_color);    
	        });	    

	        // Delay Text Boxes
	        $('#ud_box_holder').css('width', (width + 55) + "px");
	        $('#ud_box_holder').append('<form><input class="udelay_textbox" id="udtbox' + numCols + '"></form>');
	        
	        
	        numCols++;
	    }
    });            

    /*----------------------------------------------------------------------------------  
      SUBTRACT COLUMNS                                      
      ------------------------------------------------------------------------------------*/

    $('#subtract_columns').click(function(){

	    if(numCols > 1) {
	        
	        var width = parseInt($('#track_sect_holder').css('width'));
	        
	        // Track Columns
	        $('#track_sect_holder').css('width', (width - 55) +"px");  
	        $('#section' + (numCols-1)).children().remove();
	        
	        // Delay text boxes
	        $('#udtbox' + (numCols-1)).remove();
	        $('#ud_boxholder').css('width', (width - 55) + "px");

	        numBox -= 8;
	        numCols--;
	    }
    });            
});


/*-------------------------------------------------------------------------------------
  FORM ENTER REFRESH PREVENTION
  ---------------------------------------------------------------------------------------*/

$(function() {
    $("form").submit(function() { return false; });
});
