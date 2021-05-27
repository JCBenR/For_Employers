package assignment02;

import java.util.GregorianCalendar;

public class LibraryBook extends Book{
    String holder = null;
    GregorianCalendar dueDate = null;
    public LibraryBook(long isbn, String author, String title) {
        super(isbn, author, title);

        dueDate = new GregorianCalendar();
        System.out.println("The date and time are: " + dueDate);
    }

    public String getHolder(){
        return holder;
    }

    public GregorianCalendar getDueDate(){
        return dueDate;
    }
}
