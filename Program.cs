using System;
using System.Linq;
using System.Collections.Generic;
using System.Text.RegularExpressions;
using HtmlAgilityPack;
using ScrapySharp.Extensions;
using ScrapySharp.Network;
using Newtonsoft.Json;

namespace webscraping_cli
{
    class Program
    {

        static void Main(string[] args)
        {

            ScrapingBrowser browser = new ScrapingBrowser();
            int i = 0;
            bool finish = false;
            gsm gsmData = new gsm();
            while (!finish)
            {
                i++;

                WebPage mainPage = browser.NavigateToPage(new Uri($"https://www.digikala.com/search/category-mobile-phone/?pageno={i}&sortby=4"));
                HtmlNode[] phonesData = mainPage.Html
                                                    .CssSelect("#content > div > div.o-page > div > div.o-page__content > div > article > div > ul > li ")
                                                    .ToArray();

                foreach (var phoneData in phonesData)
                {
                    string data = phoneData.CssSelect("div").ToArray()[0].GetAttributeValue("data-enhanced-ecommerce");
                    Dictionary<string, string> dict = JsonConvert.DeserializeObject<Dictionary<string, string>>(data);
                    Regex re = new Regex(@"[a-zA-Z0-9- ]{4,}");
                    string name = re.Match(dict["name"]).Value.Trim();
                    int len = name.Length;

                    WebPage gsmPage = browser.NavigateToPage(new Uri($"https://www.gsmarena.com/res.php3?sSearch={name.Substring(0, len - 2)}"));
                    string url = "error";
                    try
                    {
                        url = gsmPage.Html.CssSelect("#review-body > div > ul > li > a")
                                        .ToArray()[0].GetAttributeValue("href");

                        if (dict["price"] == null)
                        {
                            finish = true;
                            break;
                        }
                        gsmData.dataExtractor(url, name, int.Parse(dict["id"]), int.Parse(dict["price"]));

                    }
                    catch (Exception e)
                    {
                        gsmData.corruptedData(name, int.Parse(dict["id"]));
                        Console.WriteLine(e.Message);
                    }

                }
            }


        }
    }
}
