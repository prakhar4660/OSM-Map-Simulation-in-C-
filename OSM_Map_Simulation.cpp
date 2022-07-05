#include <bits/stdc++.h>
#include "rapidxml.hpp"
#define ll long long int
using namespace std;
using namespace rapidxml;

//degree to radian conversion
long double toRadians(const long double degree)
{
    long double one_deg = (M_PI) / 180;
    return (one_deg * degree);
}

// Haversine formula to compute distance with lattitude - longitude information
// returns distance in metres
long double distance(long double lat1, long double long1, long double lat2, long double long2)
{
    lat1 = toRadians(lat1);
    long1 = toRadians(long1);
    lat2 = toRadians(lat2);
    long2 = toRadians(long2);

    long double dlong = long2 - long1;
    long double dlat = lat2 - lat1;

    long double ans = pow(sin(dlat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlong / 2), 2);

    ans = 2 * asin(sqrt(ans));

    long double R = 6371;

    ans = ans * R;

    return ans * 1000;
}

struct Node
{
    string id;
    long double lattitude;
    long double longitude;
    string name;
};

struct Way
{
    string id;
    string name;
};

void k_minimum_nodes(map<string, Node> &nodeMap)
{
    cout << "\n\nEnter the ID of the node for which you want to find the k-nearest nodes: ";
    ll k;
    string id;
    Node node;
    cin >> id;
    cout << "Enter the value of k: ";
    cin >> k;
    bool flag = false;

    if (nodeMap.find(id) != nodeMap.end())
    {
        node = nodeMap[id];
        flag = true;
    }

    if (!flag)
    {
        cout << "\nNo node present with the given ID\n"
             << endl;
        return;
    }
    //implemented priority queue (max heap version). it first takes first k elements from the nodelist. for the (k+1)th or later coming elements, if it is lesser than the top element of the queue, top elent is removed an this element is inserted. elemets greater than the top element of the queue are ignored
    priority_queue<pair<long double, string>> pq;

    for (auto x : nodeMap)
    {
        long double dist = distance(node.lattitude, node.longitude, x.second.lattitude, x.second.longitude);
        if (x.first != id)
        {
            if (pq.size() < k)
            {
                pq.push(make_pair(dist, x.first));
            }
            else
            {
                if (pq.top().first > dist)
                {
                    pq.pop();
                    pq.push(make_pair(dist, x.first));
                }
            }
        }
    }
    cout << "\nThe " << k << " nearest nodes are:\n\n";
    vector<pair<long double, string>> list;
    while (pq.size() > 0)
    {
        list.push_back(pq.top());
        pq.pop();
    }
    int sno = 1;
    for (auto it = list.rbegin(); it != list.rend(); it++)
    {
        cout << "S.No: " << sno++ << endl;
        cout << "Node ID: " << it->second << endl;
        if (nodeMap[it->second].name != "")
        {
            cout << "Node Name: " << nodeMap[it->second].name << endl;
        }
        cout << "Lattitude: " << nodeMap[it->second].lattitude << endl;
        cout << "Longitude: " << nodeMap[it->second].longitude << endl;
        cout << "Distance: " << it->first << "m\n\n";
    }
}
// it is a case independent implementation for searching a node.
void searchNode(map<string, Node> &nodeMap, map<string, Way> &wayMap)
{
    cout << "\n\nEnter the substring you want to search for: ";
    string key;
    cin >> key;
    transform(key.begin(), key.end(), key.begin(), ::tolower);
    bool flag = false;
    int sno = 1;
    for (auto x : nodeMap)
    {
        if (x.second.name != "")
        {
            string s1 = x.second.name;
            transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
            if (s1.find(key) != s1.npos)
            {
                if (!flag)
                {
                    cout << "\nThe search results are as follows: \n";
                }
                cout << "\nS.No: " << sno++ << endl;
                cout << "Type: Node element\n";
                cout << "Node ID: " << x.second.id << endl;
                cout << "Node Name: " << x.second.name << endl;
                cout << "Lattitude: " << x.second.lattitude << endl;
                cout << "Longitude: " << x.second.longitude << "\n";
                flag = true;
            }
        }
    }
    for (auto x : wayMap)
    {
        if (x.second.name != "")
        {
            string s1 = x.second.name;
            transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
            if (s1.find(key) != s1.npos)
            {
                cout << "\nS.No: " << sno++ << endl;
                cout << "Type: Way element\n";
                cout << "Way ID: " << x.second.id << endl;
                cout << "Way Name: " << x.second.name << "\n";
                flag = true;
            }
        }
    }
    if (!flag)
    {
        cout << "\nNO NODES FOUND!!\n\n";
    }
    else
    {
        cout << "\n";
    }
}
// recursive function to print the shortest path
void print_Path(map<string, string> parent, string src, string dest)
{
    if (dest == src)
    {
        return;
    }
    print_Path(parent, src, parent[dest]);
    cout << " -> " << dest;
}
// based in dikshtra's shortest path algorithm
void shortestPath(unordered_map<string, unordered_map<string, long double>> &graph)
{
    cout << "\n\nEnter the ID of Source node: ";
    string src, destination;
    cin >> src;
    cout << "Enter the ID of destination node: ";
    cin >> destination;
    set<pair<long double, string>> set;
    map<string, long double> dist;
    set.insert(make_pair(0, src));
    dist[src] = 0;
    map<string, string> parent;
    bool flag = false;
    while (!set.empty())
    {
        auto tmp = *set.begin();
        set.erase(set.begin());
        for (auto x : graph[tmp.second])
        {
            if (dist.find(x.first) == dist.end())
            {
                dist[x.first] = x.second + dist[tmp.second];
                parent[x.first] = tmp.second;
                set.insert(make_pair(dist[x.first], x.first));
            }
            else
            {
                if (dist[x.first] > x.second + dist[tmp.second])
                {
                    set.erase(make_pair(dist[x.first], x.first));
                    dist[x.first] = x.second + dist[tmp.second];
                    set.insert(make_pair(dist[x.first], x.first));
                    parent[x.first] = tmp.second;
                }
            }
            if (x.first == destination)
            {
                cout << "\nThe shortest distance between the two given nodes is " << dist[x.first] << " metres" << endl;
                flag = true;
                cout << "The path corresponding to above minimum distance: \n\n";
                cout << src;
                print_Path(parent, src, destination);
                cout << "\n\n";
                return;
            }
        }
    }
    if (!flag)
    {
        cout << "\nNO DATA AVAILABLE FOR THE PATH BETWEEN THESE TWO NODES\n\n";
    }
}

int main()
{
    //rapidxml parser code
    xml_document<> doc;
    xml_node<> *rootNode = nullptr;

    ifstream theFile("map.osm");

    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);

    rootNode = doc.first_node("osm");

    // parsing thorough map.osm to store sufficient information about the node and way elements
    map<string, Node> nodeMap;
    map<string, Way> wayMap;
    int nodeCount = 0, wayCount = 0;
    string element;

    for (xml_node<> *childNode = rootNode->first_node(); childNode != nullptr; childNode = childNode->next_sibling())
    {
        element = childNode->name();
        if (element == "node")
        {
            Node tmpNode;
            nodeCount++;
            xml_attribute<> *nodeAttribute = childNode->first_attribute();
            while (nodeAttribute != nullptr)
            {
                element = nodeAttribute->name();
                if (element == "id")
                {
                    tmpNode.id = nodeAttribute->value();
                }
                else if (element == "lat")
                {
                    string tmpString = nodeAttribute->value();
                    tmpNode.lattitude = stold(tmpString);
                }
                else if (element == "lon")
                {
                    string tmpString = nodeAttribute->value();
                    tmpNode.longitude = stold(tmpString);
                }
                nodeAttribute = nodeAttribute->next_attribute();
            }
            xml_node<> *tag = childNode->first_node();
            while (tag)
            {
                element = tag->first_attribute()->value();
                if (element == "name")
                {
                    tmpNode.name = tag->first_attribute()->next_attribute()->value();
                    break;
                }
                tag = tag->next_sibling();
            }
            nodeMap[tmpNode.id] = tmpNode;
        }
        else if (element == "way")
        {
            wayCount++;
            Way tmpWay;
            string s;
            xml_attribute<> *wayAttribute = childNode->first_attribute();
            while (wayAttribute)
            {
                element = wayAttribute->name();
                if (element == "id")
                {
                    tmpWay.id = wayAttribute->value();
                    break;
                }
                wayAttribute = wayAttribute->next_attribute();
            }
            xml_node<> *wayTag = childNode->first_node();
            while (wayTag)
            {
                element = wayTag->first_attribute()->value();
                if (element == "name")
                {
                    tmpWay.name = wayTag->first_attribute()->next_attribute()->value();
                    break;
                }
                wayTag = wayTag->next_sibling();
            }
            wayMap[tmpWay.id] = tmpWay;
        }
    }
    // unordered map to create the graph
    unordered_map<string, unordered_map<string, long double>> graph;
    for (xml_node<> *childNode = rootNode->first_node(); childNode != nullptr; childNode = childNode->next_sibling())
    {
        string s = childNode->name();
        if (s == "way")
        {
            string prev_id;
            long double prev_lat, prev_lon;
            bool flag = false;
            for (xml_node<> *secondChild = childNode->first_node(); secondChild; secondChild = secondChild->next_sibling())
            {
                s = secondChild->name();
                if (s == "nd")
                {
                    string id = secondChild->first_attribute()->value();
                    long double lat, lon;

                    if (nodeMap.find(id) != nodeMap.end())
                    {
                        lat = nodeMap[id].lattitude;
                        lon = nodeMap[id].longitude;

                        if (!flag)
                        {
                            prev_id = id;
                            prev_lat = lat;
                            prev_lon = lon;
                            flag = true;
                        }
                        else
                        {
                            long double dist = distance(prev_lat, prev_lon, lat, lon);
                            graph[id][prev_id] = dist;
                            graph[prev_id][id] = dist;
                            prev_id = id;
                            prev_lat = lat;
                            prev_lon = lon;
                        }
                    }
                }
            }
        }
    }

    cout << "==============================================================================================================================================================\n"
         << "\t\t\t\t\t\t\t\t\tIIT KGP MAP PARSER\n"
         << "==============================================================================================================================================================\n\n";
    cout << "\t\t\t\t\tWelcome to the software providing a text searchable interface for IIT KGP map\n\t\t\t\t\t-----------------------------------------------------------------------------\n\n";

    int option = 0;
    while (option != 5)
    {
        cout << "\n-----------------------------------------------------------------------------------------------------------------------------------------------------\n";
        cout << "Enter a number in the range [3 , 12] and choose whatever you want to do from the options below:\n\n"
             << "----> To know the total number of nodea and ways in this map, enter 1\n"
             << "----> To search for a particular node or way element by the substring of it's name, enter 2\n"
             << "----> To find the k-closest nodes to a node given by you using the crow fly distance, enter 3\n"
             << "----> To calculate the shortest path between two node elements through the way elements, enter 4\n"
             << "----> To exit the program, enter 5\n";
        cout << "-----------------------------------------------------------------------------------------------------------------------------------------------------\n\n";
        cout << "Enter a number as told above: ";
        cin >> option;
        switch (option)
        {
        case 1:
            cout << "\nNumber of nodes: " << nodeCount << endl;
            cout << "Number of ways:  " << wayCount << endl;
            break;
        case 2:
            searchNode(nodeMap, wayMap);
            break;
        case 3:
            k_minimum_nodes(nodeMap);
            break;
        case 4:
            shortestPath(graph);
            break;
        case 5:
            cout << "\n\t\t\t===================================================================================================================\n"
                 << "\t\t\tThank you for using this miniature implementation of the IIT KGP map. You have successfully exited the program now.\n"
                 << "\t\t\t===================================================================================================================\n";
            break;
        default:
            cout << "\nInvalid input!! Enter a number in the range [1 , 5]\n";
            break;
        }
    }
    return 0;
}
