using System;
using System.Linq;
using System.Collections.Generic;
using System.Text.RegularExpressions;
using HtmlAgilityPack;
using ScrapySharp.Extensions;
using ScrapySharp.Network;
using Newtonsoft.Json;
using System.Data.SqlClient;
namespace webscraping_cli
{
    public class gsm
    {
        public  SqlConnection con {get;set;}
        public SqlCommand cmd {get;set;}
        public gsm()
        {
            try
            {
                var cs = @"Server=localhost\SQLEXPRESS;Integrated Security=True;Database=master;Trusted_Connection=True;";
                con = new SqlConnection(cs);
                con.Open();

                cmd = new SqlCommand();
                cmd.Connection = con;

                cmd.CommandText = "DROP TABLE IF EXISTS Mobiles";
                cmd.ExecuteNonQuery();

                cmd.CommandText = @"CREATE TABLE Mobiles(
                ID INT NOT NULL PRIMARY KEY,   
                Model VARCHAR(255), 
                Name VARCHAR(255), 
                Search VARCHAR(255), 
                Year VARCHAR(255),  
                OS VARCHAR(255),
                Price INT,
                Battery INT,
                Chipset VARCHAR(255),
                GPU VARCHAR(255),
                Weight VARCHAR(255),
                Camera VARCHAR(255),
                Radio VARCHAR(255),
                TBench TEXT 
                )";
                cmd.ExecuteNonQuery();

            }

            catch (SqlException e)
            {
                Console.WriteLine(e.ToString());
            }
        }

        public void dataExtractor(string url,string name1, int id, int price)
        {

            ScrapingBrowser browser = new ScrapingBrowser();
            WebPage mainPage = browser.NavigateToPage(new Uri($"https://www.gsmarena.com/{url}"));
            HtmlNode[] resultsLinks = mainPage.Html
                                                .CssSelect("#specs-list >table >tr>td")
                                                .ToArray();

            Dictionary<string, string> dict = new Dictionary<string, string>();
            foreach (var data in resultsLinks)
            {
                string name = data.GetAttributeValue("data-spec");
                string info = data.InnerText;
                if (name=="" ||info == "") continue;
                dict.Add(name.Trim(), info.Trim());
            }
            string Model=mainPage.Html.CssSelect("#body > div > div.review-header > div > div.article-info-line.page-specs.light.border-bottom > h1").ToArray()[0].InnerText;
            string Battery=mainPage.Html.CssSelect("#body > div > div.review-header > div > div.center-stage.light.nobg.specs-accent > ul > li.help.accented.help-battery > strong > span").ToArray()[0].InnerText;

            dict.TryAdd("chipset","None");
            dict.TryAdd("gpu","None");
            dict.TryAdd("tbench","None");




            //Save TO DB

            try
            {
              
                cmd.CommandText = $"INSERT INTO Mobiles(ID,Model,Name,Search,Year,OS,Price,Battery,Chipset,GPU,Weight,Camera,Radio,TBench) VALUES({id},'{Model}','{name1}','{name1.Split().Where(s=>s!="").Last()}','{dict["year"]}','{dict["os"]}',{price},{int.Parse(Battery)},'{dict["chipset"]}','{dict["gpu"]}','{dict["weight"]}','{dict["cam2modules"]}','{dict["radio"]}','{dict["tbench"]}')";
                cmd.ExecuteNonQuery();
            }

            catch (SqlException e)
            {
                Console.WriteLine("sql");
                Console.WriteLine(e.ToString());
            }
        }


         public void corruptedData(string name1, int id)
        {

            //Save TO DB

            try
            {
                int len=name1.Length;
                cmd.CommandText = $"INSERT INTO Mobiles(ID,Name,Search) VALUES({id},'{name1}','{name1.Substring(0,len-2)}')";
                cmd.ExecuteNonQuery();
            }

            catch (SqlException e)
            {
                Console.WriteLine("sql");
                Console.WriteLine(e.ToString());
            }
        }
    }
}
