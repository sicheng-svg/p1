#include <iostream>
#include<unordered_map>
#include <string>
#include<algorithm>

int main(){
    std::string s = "abcabcbb";
    std::unordered_map<char, int> map;

    int n = s.size();
    int left = 0, right = 0;
    int ans = 0;
    while(right < n){
        // 进窗口
        map[s[right]]++;

        // 判断
        while(right < n && map[s[right]] > 1){
            map[s[left]]--; //出窗口
            left++;
        }
        ans = std::max(ans, right - left + 1);
        right++;
    }
    std::cout << ans;
    return 0;
}

