from sys import argv
import requests
from bs4 import BeautifulSoup
from concurrent.futures import ThreadPoolExecutor

class USER_NOT_FOUND(ValueError):
    pass

def get_problem_name(problem_id):
    url = f"https://www.spoj.com/problems/{problem_id}/"
    resp = requests.get(url)
    resp.raise_for_status()
    soup = BeautifulSoup(resp.text, "html.parser")
    submenu_div = soup.find("div", class_="submenu")
    if submenu_div:
        ol = submenu_div.find("ol")
        if ol:
            li_items = ol.find_all("li")
            if len(li_items) >= 3:
                third_li = li_items[2]
                return third_li.get_text(strip=True)
    return None

def get_solved(username):
    url = f"https://www.spoj.com/users/{username}/"
    try:
        resp = requests.get(url)
        resp.raise_for_status()
    except requests.exceptions.HTTPError as e:
        raise USER_NOT_FOUND(f"User not found: {username}") from e
    soup = BeautifulSoup(resp.text, "html.parser")

    h4 = soup.find("h4", string="List of solved classical problems:")
    if not h4:
        raise USER_NOT_FOUND(f"User not found: {username}")
    table = h4.find_next("table")
    if not table:
        return []

    # print(f"Found table for user {username}")
    problems = set()
    for row in table.find_all("tr")[1:]:
        cols = row.find_all("td")
        entries = [col.get_text(strip=True) for col in cols]
        if entries:
            problems = problems.union(entries)
    return problems

def get_problem_rank(problem_id):
    url = f'https://www.spoj.com/ranks/{problem_id}/'
    resp = requests.get(url)
    resp.raise_for_status()
    soup = BeautifulSoup(resp.text, "html.parser")
    # Find the table with class "table problems"
    table = soup.find("table", class_="table problems")
    if not table:
        raise ValueError("Table not found")
    tbody = table.find("tbody")
    if not tbody:
        raise ValueError("Table body not found")

    first_row = tbody.find("tr")
    if not first_row:
        raise ValueError("No rows found in the table body")

    first_cell = first_row.find("td")
    val = int(first_cell.get_text(strip=True))
    return val

def sort_by_rank(problems):
    with ThreadPoolExecutor() as executor:
        ranks = list(executor.map(get_problem_rank, problems))
    problems =  sorted(zip(ranks, problems))
    print(problems)
    return [problem for _, problem in problems]

def get_unsolved_from_other(user, other):
    user_solved = get_solved(user)
    other_solved = get_solved(other)
    print(len(user_solved), len(other_solved))
    unsolved = set(other_solved) - set(user_solved)
    print(unsolved)
    unsolved_list  = list(unsolved)
    unsolved_list = sort_by_rank(unsolved_list)
    return unsolved_list

if __name__ == "__main__":
    user=argv[1]
    other=argv[2]
    print(get_unsolved_from_other(user, other))