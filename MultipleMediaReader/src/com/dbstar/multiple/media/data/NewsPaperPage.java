package com.dbstar.multiple.media.data;

import java.io.Serializable;
import java.util.ArrayList;

public class NewsPaperPage implements Serializable{
    
    public String title;
    public String path;
    public String PicPath;
    public int  IsOpen = 0;
    public int SelectedIndex = -1;
    public int PaddinTop;
    public ArrayList<NewsPaperPage> mArticles = new ArrayList<NewsPaperPage>();
}
