function doExpand( para, arrow )
{
	if ( para.style.display == "none" )
	{
		para.style.display = "";
		arrow.src = "opened.gif";
	}
	else
	{
		para.style.display = "none";
		arrow.src = "closed.gif";
	}
}