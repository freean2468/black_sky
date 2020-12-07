#pragma once

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam);
int OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam);
int OnDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam);
int OnMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam);
int OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
int OnMainMenuKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
int MenuKeyCheck(WPARAM wParam);
int OnStageSelectKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
int OnGameStartKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
int OnPlayKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
int OnGameMenuKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
int OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam);
int OnLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
int OnMainMenuLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
int OnStageLButtonDown(HWND hWnd, WPARAM wParam, LPARAM);
int OnPlayLbuttonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
int OnSetCursor(HWND hWnd, WPARAM wParam, LPARAM lParam);