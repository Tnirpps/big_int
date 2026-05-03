# Contributing

Thanks for contributing to `big_int`.

## Workflow

1. Fork the repository and create a branch from `main`.
2. Make focused changes with tests when behavior changes.
3. Run the test suite locally with:

```sh
make test
```

4. Open a pull request with a short description of what changed and why.

## Notes

- Keep the library header-only.
- Prefer small, reviewable pull requests.
- If you add or change arithmetic behavior, update or add tests in `test/main.c`.
